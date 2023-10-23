#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <vector>

namespace riscv {
namespace cpu {
namespace detail {

class MemoryMonitor {
public:
    MemoryMonitor() = default;

    void Initialize(std::size_t hartCount);

    void AddReservation(std::size_t hartIndex, Address addr) noexcept;
    void ClearReservation(std::size_t hartIndex) noexcept;

    bool CheckAndKillReservation(Address addr) noexcept;

    bool ReservationExist() noexcept;
    Address GetReservation(std::size_t hartIndex) noexcept;
private:

private:
    struct Entry {
        bool active;
        Address addr;
    }; // struct Entry

    std::size_t m_ActiveCount;
    std::vector<Entry> m_Entries;
}; // class MemoryMonitor

} // namespace detail
} // namespace cpu
} // namespace riscv
