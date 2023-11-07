#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/intrpt/intrpt_ITargetCtlrBridge.h>

namespace riscv {
namespace intrpt {

/**
 * Interrupt target interface.
 * 
 * This is to be implemented by devices which wish to be able to 
 * recieve interrupt requests.
 * 
 * Instances of this are to be provided to the interrupt controller by
 * the target device.
*/
class ITarget {
public:
    virtual ~ITarget() noexcept = default;

    /**
     * This may be implemented by the target and will be called in addition
     * to setting the IRQ notified flag when an interrupt controller notifies us
     * that an interrupt request is pending.
     *
     * Note: Targets should be careful to not claim a request in their handler while still acepting
     * interrupts, as this may cause an infinite loop.
    */
    virtual Result HandleIRQ();

    /**
     * Returns whether a pending interrupt is available for this target.
    */
    bool IsPendingInterruptAvailable();

    /**
     * Notify the interrupt controller that the target won't be accepting
     * interrupt requests.
     *
     * This only acts as a hint and may be implemented by the interrupt controller as a no-op.
     *
     * Wrapper for ITargetCtlrBridge::EnableInterrupts.
    */
    Result EnableInterrupts();

    /**
     * Notify the interrupt controller that the target will be accepting
     * interrupt requests.
     *
     * This only acts as a hint and may be implemented by the interrupt controller as a no-op.
     *
     * Wrapper for ITargetCtlrBridge::DisableInterrupts.
    */
    Result DisableInterrupts();

    /**
     * Initialize the controller bridge.
     *
     * Should be only be called by a interrupt controller.
    */
    Result InitializeForController(ITargetCtlrBridge* pCtlrBridge);

    /**
     * Notifies the target that an interrupt request is available/pending.
     *
     * This should be only called by the interrupt controller.
    */
    Result NotifyIRQImpl();
private:
    bool m_IRQNotified;
    ITargetCtlrBridge* m_pCtlrBridge;
}; // class ITarget

} // namespace intrpt
} // namespace riscv
