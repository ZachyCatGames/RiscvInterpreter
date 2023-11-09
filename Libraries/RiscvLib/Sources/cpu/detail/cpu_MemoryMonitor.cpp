#include <RiscvEmu/cpu/detail/cpu_MemoryMonitor.h>
#include <RiscvEmu/diag.h>
#include <span>

namespace riscv {
namespace cpu {
namespace detail {

namespace {

constexpr Address GetAlignedAddress(Address addr) noexcept { return addr % sizeof(NativeWord); }

} // namespace

MemoryMonitor::Context::Context() noexcept :
    m_pParent(nullptr), m_HartId(0) {}

MemoryMonitor::Context::Context(MemoryMonitor* pParent, Word hartId) noexcept :
    m_pParent(pParent), m_HartId(hartId) {}

void MemoryMonitor::Context::AquireReservation(Address addr) noexcept {
    diag::AssertNotNull(m_pParent);
    return m_pParent->AquireReservation(m_HartId, addr);
}

void MemoryMonitor::Context::ReleaseReservation() noexcept {
    diag::AssertNotNull(m_pParent);
    return m_pParent->ReleaseReservation(m_HartId);
}

void MemoryMonitor::Context::AquireSharedAccess(Address addr) {
    diag::AssertNotNull(m_pParent);
    return m_pParent->AquireSharedAccess(m_HartId, addr);
}

void MemoryMonitor::Context::ReleaseSharedAccess() {
    diag::AssertNotNull(m_pParent);
    return m_pParent->ReleaseSharedAccess(m_HartId);
}

bool MemoryMonitor::Context::HasReservation() const noexcept {
    diag::AssertNotNull(m_pParent);
    return m_pParent->HartHasReservation(m_HartId);
}

Address MemoryMonitor::Context::GetReservedAddress() const noexcept {
    diag::AssertNotNull(m_pParent);
    return m_pParent->HartGetReservedAddress(m_HartId);
}

bool MemoryMonitor::Context::IsAddressReserved(Address addr) const noexcept {
    diag::AssertNotNull(m_pParent);
    return m_pParent->IsAddressReserved(addr);
}

bool MemoryMonitor::Context::TryRevokeAnyReservation(Address addr) noexcept {
    diag::AssertNotNull(m_pParent);
    return m_pParent->TryRevokeReservation(addr);
}

void MemoryMonitor::Initialize(Word hartCount) {
    m_HartCount = hartCount;

    /* Clear counters. */
    m_ReserveCount      = 0;
    m_SharedAccessCount = 0;
    m_ExclAccessCount   = 0;

    /* Setup entries. */
    m_ReservEntries = std::make_unique<ReservationEntry[]>(hartCount);
    m_SharedEntries = std::make_unique<SharedAccessEntry[]>(hartCount);
    m_ExclEntries   = std::make_unique<ExclAccessEntry[]>(hartCount);
}

MemoryMonitor::Context MemoryMonitor::GetContext(Word hartId) noexcept { return Context(this, hartId); }

void MemoryMonitor::AquireReservation(Word hartId, Address addr) noexcept {
    diag::Assert(hartId < m_HartCount);

    /* Align reservation address. */
    addr = GetAlignedAddress(addr);

    /* Revoke any current reservations of this address. */
    this->TryRevokeReservation(addr);

    /* Mark entry as active if required. */
    auto& entry = m_ReservEntries[hartId];
    if(!entry.active) {
        m_ReserveCount++;
        entry.active = true;
    }

    /* Reserve address. */
    entry.addr = addr;
}

void MemoryMonitor::ReleaseReservation(Word hartId) noexcept {
    diag::Assert(hartId < m_HartCount);

    auto& entry = m_ReservEntries[hartId];
    if(entry.active) {
        entry.active = false;
        m_ReserveCount--;
    }
}

void MemoryMonitor::AquireSharedAccess(Word hartIndex, Address addr) {
    diag::Assert(hartIndex < m_HartCount);

    std::unique_lock exclLock(m_ExclAccessMutex);

    /* If we have any active exclusive access reservations, check if addr is in one of them. */
    if(m_ExclAccessCount) {
        bool foundExcl = true;
        while(foundExcl) {
            foundExcl = false;
            for(auto& entry : std::span(m_ExclEntries.get(), m_HartCount)) {
                if(entry.active && entry.addr == addr) {
                    /* Release exclusive access lock. */
                    exclLock.unlock();

                    /* Indicate that an exclusive access at this address was found and we need to re-iterate. */
                    foundExcl = true;

                    /* Wait for exclusive access to be released. */
                    std::unique_lock entryLock(entry.mutex);
                    while(entry.active) {
                        entry.cv.wait(entryLock);
                    }
                    entryLock.unlock();

                    /* Reaquire exclusive access lock. */
                    exclLock.lock();

                    break;
                }
            }
        }
    }

    /* Aquire shared access mutex. */
    std::scoped_lock sharedLock(m_SharedAccessMutex);

    /* Release exclusive access mutex. */
    exclLock.unlock();

    /* Assert this hart doesn't already have an active shared access reservation. */
    auto& entry = m_SharedEntries[hartIndex];
    diag::Assert(!entry.active, "Hart already has active shared access reservation!\n");

    /* Add shared access reservation. */
    entry.active = true;
    entry.addr = addr;
    m_SharedAccessCount++;
}

void MemoryMonitor::ReleaseSharedAccess(Word hartIndex) {
    diag::Assert(hartIndex < m_HartCount);
    auto& entry = m_SharedEntries[hartIndex];

    /* Release shared access. */
    {
        std::scoped_lock lock(entry.mutex);
        entry.active = false;
    }

    /* Decrement shared access counter. */
    {
        std::scoped_lock lock(m_SharedAccessMutex);
        m_SharedAccessCount--;
    }

    /* Signal anyone waiting on us. */
    entry.cv.notify_one();
}

void MemoryMonitor::AquireExclusiveAccess(Word hartIndex, Address addr) {
    diag::Assert(hartIndex < m_HartCount);

    /* Aquire both locks. */
    std::unique_lock exclLock(m_ExclAccessMutex);
    std::unique_lock sharedLock(m_SharedAccessMutex);

    /* If we have any shared access reservations, check if addr is in one of them. */
    while(m_SharedAccessCount) {
        bool foundShared = true;
        while(foundShared) {
            foundShared = false;
            for(auto& entry : std::span(m_SharedEntries.get(), m_HartCount)) {
                if(entry.active && entry.addr == addr) {
                    /* Release both locks. */
                    exclLock.unlock();
                    sharedLock.unlock();

                    /* Indicate that a shared access reservation was found at this address and we'll need to reiterate. */
                    foundShared = true;

                    /* Wait for shared access reservation to be released. */
                    std::unique_lock entryLock(entry.mutex);
                    while(entry.active) {
                        entry.cv.wait(entryLock);
                    }
                    entryLock.unlock();

                    /* Reaquire locks. */
                    exclLock.lock();
                    sharedLock.lock();

                    break;
                }
            }
        }
    }

    /* Assert this hart doesn't already have an active exclusive access reservation. */
    auto& entry = m_ExclEntries[hartIndex];
    diag::Assert(!entry.active, "Hart already has active exclusive access reservation!\n");

    /* Add exclusive access reservation. */
    entry.active = true;
    entry.addr = addr;
    m_ExclAccessCount++;
}

void MemoryMonitor::ReleaseExclusiveAccess(Word hartIndex) {
    diag::Assert(hartIndex < m_HartCount);
    auto& entry = m_ExclEntries[hartIndex];

    /* Release exclusive access reservation. */
    {
        std::scoped_lock lock(entry.mutex);
        entry.active = false;
    }

    /* Decrement exclusive access reservation count. */
    {
        std::scoped_lock lock(m_ExclAccessMutex);
        m_ExclAccessCount--;
    }

    /* Signal anyone waiting on us. */
    entry.cv.notify_one();
}

bool MemoryMonitor::HartHasReservation(Word hartId) const noexcept {
    diag::Assert(hartId < m_HartCount);
    return m_ReservEntries[hartId].active;
}
Address MemoryMonitor::HartGetReservedAddress(Word hartId) const noexcept {
    diag::Assert(hartId < m_HartCount);
    return m_ReservEntries[hartId].addr;
}

bool MemoryMonitor::IsAddressReserved(Address addr) const noexcept {
    /* Align address. */
    addr = GetAlignedAddress(addr);

    /* Search for an active entry using this address. */
    for(const auto& entry : std::span(m_ReservEntries.get(), m_HartCount)) {
        if(entry.active && entry.addr == addr) {
            return true;
        }
    }

    return false;
}

bool MemoryMonitor::TryRevokeReservation(Address addr) noexcept {
    /* Align address. */
    addr = GetAlignedAddress(addr);

    /* Simply quit if we have no active entries. */
    if(!m_ReserveCount) {
        return false;
    }

    /* Search for any active entry using this address and revoke it. */
    for(auto& entry : std::span(m_ReservEntries.get(), m_HartCount)) {
        if(entry.active && entry.addr == addr) {
            entry.active = false;
            m_ReserveCount--;
            return true;
        }
    }

    return false;
}

} // namespace detail
} // namespace cpu
} // namespace riscv
