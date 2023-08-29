#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/mem/detail/mem_RegionBase.h>

namespace riscv {
namespace mem {

enum class RegionType {
    Memory,
    IO
}; // enum class RegionType

class RegionInfo : public detail::RegionBase {
public:
    constexpr RegionInfo(Address addr, NativeWord length, RegionType type) noexcept :
        RegionBase(addr, length),
        m_Type(type) {}

    constexpr auto GetType() const noexcept { return m_Type; }
private:
    RegionType m_Type;
}; // class RegionInfo

} // namespace mem
} // namespace riscv
