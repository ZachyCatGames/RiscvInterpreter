#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/result.h>

namespace riscv {
namespace hw {

/**
 * This interface represents a hardware device that updates at a given frequency.
 *
 * See hw::DeviceScheduler.
*/
class IDevice {
public:
    constexpr IDevice(DWordS updateFreq) noexcept :
        m_UpdateFreq(updateFreq) {}

    constexpr auto GetFreq() const noexcept { return m_UpdateFreq; }

    constexpr virtual Result ProcessCycle() = 0;
private:
    DWordS m_UpdateFreq;
}; // class IDevice

} // namespace hw
} // namespace riscv
