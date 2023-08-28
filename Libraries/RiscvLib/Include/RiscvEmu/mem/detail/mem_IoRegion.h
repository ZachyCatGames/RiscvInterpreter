#pragma once
#include <RiscvEmu/mem/detail/mem_RegionBase.h>
#include <RiscvEmu/mem/mem_IMmioDev.h>
#include <RiscvEmu/mem/mem_RegionInfo.h>
#include <vector>
#include <memory>

namespace riscv {
namespace mem {
namespace detail {

class IoDev : public RegionBase {
public:
    constexpr IoDev(Address addr, NativeWord len, std::unique_ptr<IMmioDev>&& pDev) :
        RegionBase(addr, len),
        pDev(std::move(pDev)) {}

    constexpr IMmioDev* GetDevice() noexcept { return pDev.get(); }
    constexpr const IMmioDev* GetDevice() const noexcept { return pDev.get(); }
private:
    std::unique_ptr<IMmioDev> pDev;
}; // class IoDev

class IoRegion : public RegionBase {
public:
    constexpr IoRegion(const RegionInfo& info) :
        RegionBase(info.start, info.length) {}

    constexpr auto& GetDevList() noexcept { return m_Devices; }
    constexpr const auto& GetDevList() const noexcept { return m_Devices; }

    constexpr IMmioDev* FindDevice(Address addr) {
        for(auto& dev : m_Devices) {
            if(dev.Includes(addr)) {
                return dev.GetDevice();
            }
        }
        return nullptr;
    }
private:
    std::vector<IoDev> m_Devices;
}; // class IoRegion

} // namespace detail
} // namespace mem
} // namespace riscv
