#pragma once
#include <RiscvEmu/mem/mem_RegionInfo.h>
#include <RiscvEmu/mem/mem_Result.h>
#include <RiscvEmu/mem/detail/mem_RegionBase.h>
#include <RiscvEmu/mem/detail/mem_MemoryDeviceImpl.h>
#include <memory>

namespace riscv {
namespace mem {
namespace detail {

class MemRegion : public RegionBase {
public:
    MemRegion() noexcept;

    void Initialize(const RegionBase& info);

    template<typename WordType>
    Result Load(WordType* pOut, Address addr) { return m_Dev.LoadImpl<WordType>(pOut, addr); }

    template<typename WordType>
    Result Store(WordType in, Address addr) { return m_Dev.StoreImpl<WordType>(in, addr); }
private:
    MemoryDeviceImpl m_Dev;
}; // class MemRegion

} // namespace detail
} // namespace mem
} // namespace riscv
