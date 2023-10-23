#include <RiscvEmu/cpu/detail/cpu_MemoryMonitor.h>

namespace riscv {
namespace cpu {
namespace detail {

MemoryMonitor::MemoryMonitor(std::size_t hartCount) :
    m_ActiveCount(0), m_Entries(hartCount) {}

void MemoryMonitor::AddReservation(std::size_t hartIndex, Address addr) noexcept {
    auto& entry = m_Entries[hartIndex];

    if(!entry.active) {
        m_ActiveCount++;
    }

    entry.active = true;
    entry.addr = addr;
}

void MemoryMonitor::ClearReservation(std::size_t hartIndex) noexcept {
    auto& entry = m_Entries[hartIndex];

    if(entry.active) {
        entry.active = false;
        m_ActiveCount--;
    }
}

bool MemoryMonitor::CheckAndKillReservation(Address addr) noexcept {
    if(!m_ActiveCount) {
        return false;
    }

    for(auto& entry : m_Entries) {
        if(entry.active && entry.addr == addr) {
            entry.active = false;
            return true;
        }
    }

    return false;
}

bool MemoryMonitor::ReservationExist() noexcept {
    return m_ActiveCount;
}

Address MemoryMonitor::GetReservation(std::size_t hartIndex) noexcept {
    auto& entry = m_Entries[hartIndex];

    if(entry.active) {
        return entry.addr;
    }

    return 0;
}

} // namespace detail
} // namespace cpu
} // namespace riscv
