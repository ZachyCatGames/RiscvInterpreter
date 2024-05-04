#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/mem/mem_IMmioDev.h>
#include <RiscvEmu/mem/mem_MCClient.h>
#include <RiscvEmu/mem/mem_RegionInfo.h>
#include <RiscvEmu/mem/mem_Result.h>
#include <RiscvEmu/mem/detail/mem_MemRegion.h>
#include <RiscvEmu/mem/detail/mem_IoRegion.h>
#include <array>
#include <mutex>
#include <vector>

namespace riscv {
namespace mem {

/**
 * This represents a Memory Controller/MMIO Controller device.
 * 
 * Two types of memory regions are supported: IO regions and a single main memory/ram region.
 * 
 * If multiple ram regions are needed a mem::MemoryDevice mmio dev class is provided and may 
 * be added as an mmio device like any other mmio device.
 * 
 * Any time a memory access is performed the address will be first checked against the main
 * memory region, if it's not found there it'll be searched for in each IO region. If it's not
 * found it either ResultReadAccessFault/ResultWriteAccessFault will be returned.
*/
class MemoryController {
public:
    /** Default constructor, initializes with empty state. */
    MemoryController();

    /**
     * Initializes the Memory Controller's regions.
     * 
     * Note: Only a single Memory region may exist, if multiple entries are present the last will be used.
     * If multiple memory regions are needed, add a mem::Ram IO device.
     * 
     * @param[in] regions  Regions to add.
     * @return ResultRegionAlreadyExists() if a region in pRegions overlaps with a prexisting region.
     * @return ResultInvalidRegionType() if the type field in an entry is invalid.
     * @return ResultSuccess() otherwise.
    */
    template<typename T>
    requires std::same_as<typename T::value_type, RegionInfo>
    Result Initialize(const T& regions) {
        Result res;
        for(const auto& region : regions) {
            res = this->AddRegionImpl(&region);
            if(res.IsFailure()) {
                break;
            }
        }

        return ResultSuccess();
    }

    Result Initialize(const RegionInfo* pRegions, std::size_t count);

    /** Add an MMIO device. */
    Result AddMmioDev(IMmioDev* dev, Address addr);

    template<typename WordType>
    Result Load(WordType* pOut, Address addr) { return this->LoadImpl(pOut, addr); }

    template<typename WordType>
    Result Store(WordType in, Address addr) { return this->StoreImpl(in, addr); }

    template<typename WordType>
    Result LoadReserve(int client, WordType* pOut, Address addr);

    template<typename WordType>
    Result StoreConditional(int client, WordType* pInOut, Address addr);
private:
    friend class MemoryClient;

    static constexpr auto MaxClientCount = 16; // TODO: Control with macro

    struct ReserveEntry {
        Address addr;
        int client;
    }; // struct ReserveEntry

    /* Number of least significant bits to cut off. */
    static constexpr auto ReserveGranularity = 3; // align to 8 bytes.

    std::mutex m_ReserveMutex;
    std::vector<ReserveEntry> m_Reservations;

    MCClient m_Clients[MaxClientCount];

    detail::MemRegion m_MemRegion;
    std::vector<detail::IoRegion> m_IoRegions;
private:
    Result AddRegionImpl(const RegionInfo* pRegion);

    template<std::unsigned_integral WordType>
    Result LoadImpl(WordType* pOut, Address addr);

    template<std::signed_integral WordType>
    Result LoadImpl(WordType* pOut, Address addr);

    template<std::floating_point WordType>
    Result LoadImpl(WordType* pOut, Address addr);

    template<std::unsigned_integral WordType>
    Result StoreImpl(WordType in, Address addr);

    template<std::signed_integral WordType>
    Result StoreImpl(WordType in, Address addr);

    template<std::floating_point WordType>
    Result StoreImpl(WordType in, Address addr);

    bool HasReservation(int client, Address addr);
    void AddReservation(int client, Address addr);
    void InvalidateReservation(Address addr);

    template<typename T>
    T* FindRegionImpl(std::vector<T>& regionList, Address addr);

    detail::IoRegion* FindIoRegion(Address addr);
    detail::IoDev* FindIoDevice(Address addr, NativeWord len);
}; // class MemoryController

} // namespace mem
} // namespace riscv
