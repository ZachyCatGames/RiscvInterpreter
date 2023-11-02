#include <RiscvEmu/intrpt/intrpt_ITarget.h>

namespace riscv {
namespace intrpt {

Result ITarget::InitializeForController(ITargetCtlrBridge* pCtlrBridge) {
    m_pCtlrBridge = pCtlrBridge;
    return ResultSuccess();
}

Result ITarget::EnableInterrupts() {
    return m_pCtlrBridge->EnableInterrupts();
}

Result ITarget::DisableInterrupts() {
    return m_pCtlrBridge->DisableInterrupts();
}

} // namespace intrpt
} // namespace riscv
