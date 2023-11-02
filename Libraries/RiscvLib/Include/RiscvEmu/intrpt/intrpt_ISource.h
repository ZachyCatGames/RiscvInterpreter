#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace intrpt {

/**
 * Interrupt source interface.
 * 
 * This is to be implemented by the interrupt controller device.
 * 
 * Instances of this are to be provided to source devices which 
 * wish to be able to signal interrupts.
*/
class ISource {
public:
    virtual ~ISource() noexcept = default;

    virtual Result SignalInterrupt() = 0;
}; // class ISource

} // namespace intrpt
} // namespace riscv
