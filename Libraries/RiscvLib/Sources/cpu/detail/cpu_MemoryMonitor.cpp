#include <RiscvEmu/cpu/detail/cpu_MemoryMonitor.h>
#include <cassert>

namespace riscv {
namespace cpu {
namespace detail {

namespace {

constexpr Address GetAlignedAddress(Address addr) noexcept { return addr % sizeof(NativeWord); }

} // namespace

MemoryMonitor::Context::Context() noexcept :
    m_pParent(nullptr), m_HartId(0) {}

MemoryMonitor::Context::Context(MemoryMonitor* pParent, std::size_t hartId) noexcept :
    m_pParent(pParent), m_HartId(hartId) {}

void MemoryMonitor::Context::AddReservation(Address addr) noexcept {
    assert(m_pParent);
    return m_pParent->AddReservation(m_HartId, addr);
}

void MemoryMonitor::Context::ClearReservation() noexcept {
    assert(m_pParent);
    return m_pParent->ClearReservation(m_HartId);
}

bool MemoryMonitor::Context::HasReservation() const noexcept {
    assert(m_pParent);
    return m_pParent->HartHasReservation(m_HartId);
}

Address MemoryMonitor::Context::GetReservedAddress() const noexcept {
    assert(m_pParent);
    return m_pParent->HartGetReservedAddress(m_HartId);
}

bool MemoryMonitor::Context::IsAddressReserved(Address addr) const noexcept {
    assert(m_pParent);
    return m_pParent->IsAddressReserved(addr);
}

bool MemoryMonitor::Context::TryRevokeAnyReservation(Address addr) noexcept {
    assert(m_pParent);
    return m_pParent->TryRevokeReservation(addr);
}

void MemoryMonitor::Initialize(std::size_t hartCount) {
    m_ActiveCount = 0;
    m_Entries.resize(hartCount);
}

MemoryMonitor::Context MemoryMonitor::GetContext(std::size_t hartId) noexcept { return Context(this, hartId); }

void MemoryMonitor::AddReservation(std::size_t hartId, Address addr) noexcept {
    assert(hartId < m_Entries.size());

    /* Align reservation address. */
    addr = GetAlignedAddress(addr);

    /* Revoke any current reservations of this address. */
    this->TryRevokeReservation(addr);

    auto& entry = m_Entries[hartId];

    /* Mark entry as active if required. */
    if(!entry.active) {
        m_ActiveCount++;
        entry.active = true;
    }

    /* Reserve address. */
    entry.addr = addr;
}

void MemoryMonitor::ClearReservation(std::size_t hartId) noexcept {
    assert(hartId < m_Entries.size());
    auto& entry = m_Entries[hartId];

    if(entry.active) {
        entry.active = false;
        m_ActiveCount--;
    }
}

bool MemoryMonitor::HartHasReservation(std::size_t hartId) const noexcept {
    assert(hartId < m_Entries.size());
    return m_Entries[hartId].active;
}
Address MemoryMonitor::HartGetReservedAddress(std::size_t hartId) const noexcept {
    assert(hartId < m_Entries.size());
    return m_Entries[hartId].addr;
}

bool MemoryMonitor::IsAddressReserved(Address addr) const noexcept {
    /* Align address. */
    addr = GetAlignedAddress(addr);

    /* Search for an active entry using this address. */
    for(const auto& entry : m_Entries) {
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
    if(!m_ActiveCount) {
        return false;
    }

    /* Search for any active entry using this address and revoke it. */
    for(auto& entry : m_Entries) {
        if(entry.active && entry.addr == addr) {
            entry.active = false;
            m_ActiveCount--;
            return true;
        }
    }

    return false;
}

} // namespace detail
} // namespace cpu
} // namespace riscv
