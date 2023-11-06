#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <vector>

namespace riscv {
namespace cpu {
namespace detail {

class MemoryMonitor {
private:
    struct Entry;
public:
    class Context {
    public:
        Context() noexcept;

        void AddReservation(Address addr) noexcept;
        void ClearReservation() noexcept;

        bool HasReservation() const noexcept;
        Address GetReservedAddress() const noexcept;

        bool IsAddressReserved(Address addr) const noexcept;

        /** Revoke a reservation at an address belonging to any hart. */
        bool TryRevokeAnyReservation(Address addr) noexcept;
    private:
        friend class MemoryMonitor;
        Context(MemoryMonitor* pParent, std::size_t hartId) noexcept;
    private:
        MemoryMonitor* m_pParent;
        std::size_t m_HartId;
    }; // class Context
public:
    MemoryMonitor() = default;

    void Initialize(std::size_t hartCount);

    Context GetContext(std::size_t hartId) noexcept;
private:
    void AddReservation(std::size_t hartIndex, Address addr) noexcept;
    void ClearReservation(std::size_t hartIndex) noexcept;

    bool HartHasReservation(std::size_t hartId) const noexcept;
    Address HartGetReservedAddress(std::size_t hartId) const noexcept;

    bool IsAddressReserved(Address addr) const noexcept;

    bool TryRevokeReservation(Address addr) noexcept;
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
