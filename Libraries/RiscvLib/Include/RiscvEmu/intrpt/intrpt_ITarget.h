#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/intrpt/detail/intrpt_ITargetForCtrl.h>

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
     * This may be implemented by target devices and will be called whenever
     * an interrupt the target has enabled is pending.
     * 
     * Note: Targets should not claim a request within this.
    */
    virtual Result NotifyAvailableIRQ();

    /**
     * This updates the internal pending flag and returns whether an interrupt is
     * pending for this target.
    */
    bool HasPendingIRQ();

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

    bool HasEnabledIRQ(int src);
private:
    friend class detail::ITargetForCtrl;

    /**
     * Initialize the controller bridge.
     *
     * Should be only be called by a interrupt controller.
    */
    void InitializeForController(detail::ITargetForCtrl* pCtlrBridge);

    /**
     * Notifies the target that an interrupt request is available/pending.
     *
     * This should be only called by the interrupt controller.
    */
    Result NotifyAvailableIRQImpl();
private:
    bool m_PendingIRQ;
    detail::ITargetForCtrl* m_pCtlrBridge;
}; // class ITarget

} // namespace intrpt
} // namespace riscv
