#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace intrpt {

class ITargetCtlrBridge {
public:
    /**
     * Notify the interrupt controller that the target won't be accepting
     * interrupt requests.
    */
    virtual Result EnableInterrupts() = 0;

    /**
     * Notify the interrupt controller that the target will be accepting
     * interrupt requests.
    */
    virtual Result DisableInterrupts() = 0;

    /**
     * Returns whether an interrupt is currently pending.
    */
    virtual bool IsPendingInterruptAvailable() = 0;
}; // class ITargetCtlrBridge

} // namespace riscv
} // namespace intrpt