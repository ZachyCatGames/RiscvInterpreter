#pragma once
#include <RiscvEmu/mem/mem_RegionInfo.h>
#include <RiscvEmu/mem/mem_Result.h>
#include <RiscvEmu/mem/detail/mem_RegionBase.h>
#include <RiscvEmu/mem/detail/mem_MemoryDeviceImpl.h>
#include <memory>

namespace riscv {
namespace mem {
namespace detail {

class MemRegion : public RegionBase, private MemoryDeviceImpl<std::unique_ptr<Byte[]>> {
public:
    MemRegion() noexcept = default;
    constexpr MemRegion(const RegionBase& info) :
        RegionBase(info),
        MemoryDeviceImpl(std::make_unique<Byte[]>(info.GetLength())) {}

    constexpr void Initialize(const RegionBase& info) {
        RegionBase::Initialize(info);
        MemoryDeviceImpl::Initialize(std::make_unique<Byte[]>(info.GetLength()));
    }

    constexpr Result ReadByte  (Byte* pOut, Address addr)  { return this->ReadByteImpl(pOut, addr); }
    constexpr Result ReadHWord (HWord* pOut, Address addr) { return this->ReadHWordImpl(pOut, addr); }
    constexpr Result ReadWord  (Word* pOut, Address addr)  { return this->ReadWordImpl(pOut, addr); }
    constexpr Result ReadDWord (DWord* pOut, Address addr) { return this->ReadDWordImpl(pOut, addr); }
    constexpr Result WriteByte (Byte in, Address addr)     { return this->WriteByteImpl(in, addr); }
    constexpr Result WriteHWord(HWord in, Address addr)    { return this->WriteHWordImpl(in, addr); }
    constexpr Result WriteWord (Word in, Address addr)     { return this->WriteWordImpl(in, addr); }
    constexpr Result WriteDWord(DWord in, Address addr)    { return this->WriteDWordImpl(in, addr); }
}; // class MemRegion

} // namespace detail
} // namespace mem
} // namespace riscv
