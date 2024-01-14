#include <RiscvEmu/intrpt/intrpt_ITarget.h>

namespace riscv {
namespace intrpt {

void ITarget::InitializeForController(detail::ITargetForCtrl* pCtlrBridge) { m_pCtlrBridge = pCtlrBridge; }

Result ITarget::NotifyAvailableIRQ() { return ResultSuccess(); }

bool ITarget::HasPendingIRQ() {
    /* If we think we have an interrupt available, double check. */
    if(m_PendingIRQ) {
        m_PendingIRQ = m_pCtlrBridge->HasPendingIRQ();
    }

    return m_PendingIRQ;
}

Result ITarget::EnableInterrupts() {
    return m_pCtlrBridge->EnableInterrupts();
}

Result ITarget::DisableInterrupts() {
    return m_pCtlrBridge->DisableInterrupts();
}

Result ITarget::NotifyAvailableIRQImpl() {
    /* Set notified flag. */
    m_PendingIRQ = true;

    /* Call IRQ handler function. */
    return this->NotifyAvailableIRQ();
}

} // namespace intrpt
} // namespace riscv
