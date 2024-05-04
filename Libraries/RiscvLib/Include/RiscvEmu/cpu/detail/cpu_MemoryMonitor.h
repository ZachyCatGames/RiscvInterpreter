#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <condition_variable>
#include <memory>
#include <mutex>

namespace riscv {
namespace cpu {
namespace detail {

class MemoryMonitor {
public:
    class Context {
    public:
        Context() noexcept;

        void AquireReservation(Address addr) noexcept;
        void ReleaseReservation() noexcept;

        void AquireSharedAccess(Address addr);
        bool TryAquireSharedAccess(Address addr);
        void ReleaseSharedAccess();

        void AquireExclusiveAccess(Address addr);
        bool TryAquireExclusiveAccess(Address addr);
        void ReleaseExclusiveAccess();

        bool HasReservation() const noexcept;
        Address GetReservedAddress() const noexcept;

        bool IsAddressReserved(Address addr) const noexcept;

        /** Revoke a reservation at an address belonging to any hart. */
        bool TryRevokeAnyReservation(Address addr) noexcept;
    private:
        friend class MemoryMonitor;
        Context(MemoryMonitor* pParent, Word hartId) noexcept;
    private:
        MemoryMonitor* m_pParent;
        Word m_HartId;
    }; // class Context
public:
    MemoryMonitor() = default;
    MemoryMonitor(const MemoryMonitor&) = delete;
    MemoryMonitor(MemoryMonitor&&) = delete;

    void Initialize(Word hartCount);
    void Finalize();

    Context GetContext(Word hartId) noexcept;
private:
    void AquireReservation(Word hartIndex, Address addr) noexcept;
    void ReleaseReservation(Word hartIndex) noexcept;

    void AquireSharedAccessImpl(Word hartIndex, Address addr);
    void AquireSharedAccess(Word hartIndex, Address addr);
    bool TryAquireSharedAccess(Word hartIndex, Address addr);
    void ReleaseSharedAccess(Word hartIndex);

    void AquireExclusiveAccessImpl(Word hartIndex, Address addr);
    void AquireExclusiveAccess(Word hartIndex, Address addr);
    bool TryAquireExclusiveAccess(Word hartIndex, Address addr);
    void ReleaseExclusiveAccess(Word hartIndex);

    bool HartHasReservation(Word hartId) const noexcept;
    Address HartGetReservedAddress(Word hartId) const noexcept;

    bool IsAddressReserved(Address addr) const noexcept;

    bool TryRevokeReservation(Address addr) noexcept;
private:
    struct ReservationEntry {
        bool active;
        Address addr;
    }; // struct Entry

    struct SharedAccessEntry {
        bool active;
        Address addr;
        std::mutex mutex;
        std::condition_variable cv;
    }; // struct SharedAccessEntry

    struct ExclAccessEntry {
        bool active;
        Address addr;
        std::mutex mutex;
        std::condition_variable cv;
    }; // struct ExclAccessEntry

    std::mutex m_ExclAccessMutex;
    std::mutex m_SharedAccessMutex;

    Word m_HartCount;

    std::size_t m_ReserveCount;
    std::unique_ptr<ReservationEntry[]> m_ReservEntries;

    std::size_t m_SharedAccessCount;
    std::unique_ptr<SharedAccessEntry[]> m_SharedEntries;

    std::size_t m_ExclAccessCount;
    std::unique_ptr<ExclAccessEntry[]> m_ExclEntries;
}; // class MemoryMonitor

} // namespace detail
} // namespace cpu
} // namespace riscv
