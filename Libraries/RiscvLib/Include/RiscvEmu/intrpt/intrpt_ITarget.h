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
     * Receive an interrupt request.
    */
    virtual Result ReceiveInterruptRequest() = 0;

    /**
     * Notify the interrupt controller that the target won't be accepting
     * interrupt requests.
     * 
     * Wrapper for ITargetCtlrBridge::EnableInterrupts.
    */
    Result EnableInterrupts();

    /**
     * Notify the interrupt controller that the target will be accepting
     * interrupt requests.
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
private:
    ITargetCtlrBridge* m_pCtlrBridge;
}; // class ITarget

} // namespace intrpt
} // namespace riscv
