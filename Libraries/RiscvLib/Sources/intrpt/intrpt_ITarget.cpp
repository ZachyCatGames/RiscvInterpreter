#include <RiscvEmu/intrpt/intrpt_ITarget.h>

namespace riscv {
namespace intrpt {

Result ITarget::InitializeForController(ITargetCtlrBridge* pCtlrBridge) {
    m_pCtlrBridge = pCtlrBridge;
    return ResultSuccess();
}

Result ITarget::HandleIRQ() { return ResultSuccess(); }

bool ITarget::IsPendingInterruptAvailable() {
    /* Check if we've been notified of interrupt, if we have ask the interrupt controller
     * if a request is still available, since asking the controller is relatively expensive. */
    if(m_IRQNotified) {
        m_IRQNotified = m_pCtlrBridge->IsPendingInterruptAvailable();
    }

    return m_IRQNotified;
}

Result ITarget::EnableInterrupts() {
    return m_pCtlrBridge->EnableInterrupts();
}

Result ITarget::DisableInterrupts() {
    return m_pCtlrBridge->DisableInterrupts();
}

Result ITarget::NotifyIRQImpl() {
    /* Set notified flag. */
    m_IRQNotified = true;

    /* Call IRQ handler function. */
    return this->HandleIRQ();
}

} // namespace intrpt
} // namespace riscv
