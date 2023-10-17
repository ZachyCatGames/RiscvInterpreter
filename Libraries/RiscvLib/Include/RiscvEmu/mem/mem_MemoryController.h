#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/mem/mem_IMmioDev.h>
#include <RiscvEmu/mem/mem_RegionInfo.h>
#include <RiscvEmu/mem/mem_Result.h>
#include <RiscvEmu/mem/detail/mem_MemRegion.h>
#include <RiscvEmu/mem/detail/mem_IoRegion.h>
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

    /**
     * Initializes the Memory Controller's regions.
     * 
     * Note: Only a single Memory region may exist, if multiple entries are present the last will be used.
     * If multiple memory regions are needed, add a mem::Ram IO device.
     * 
     * @param[in] pRegions  Regions to add.
     * @param[in] regionCount  Number of entries in pRegions.
     * @return ResultRegionAlreadyExists() if a region in pRegions overlaps with a prexisting region.
     * @return ResultInvalidRegionType() if the type field in an entry is invalid.
     * @return ResultSuccess() otherwise.
    */
    Result Initialize(const RegionInfo* pRegions, std::size_t regionCount);

    /** Add an MMIO device. */
    Result AddMmioDev(IMmioDev* dev, Address addr);

    Result ReadByte(Byte* pOut, Address addr);

    Result ReadHWord(HWord* pOut, Address addr);

    Result ReadWord(Word* pOut, Address addr);

    Result ReadDWord(DWord* pOut, Address addr);

    Result ReadNativeWord(NativeWord* pOut, Address addr);

    Result WriteByte(Byte in, Address addr);

    Result WriteHWord(HWord in, Address addr);

    Result WriteWord(Word in, Address addr);

    Result WriteDWord(DWord in, Address addr);

    Result WriteNativeWord(NativeWord in, Address addr);
private:
    detail::MemRegion m_MemRegion;
    std::vector<detail::IoRegion> m_IoRegions;
private:
    template<auto MemRead, auto IoRead, typename T>
    Result ReadWriteImpl(T pOut, Address addr);

    template<typename T>
    T* FindRegionImpl(std::vector<T>& regionList, Address addr);

    detail::IoRegion* FindIoRegion(Address addr);
    detail::IoDev* FindIoDevice(Address addr, NativeWord len);
};

} // namespace mem
} // namespace riscv
