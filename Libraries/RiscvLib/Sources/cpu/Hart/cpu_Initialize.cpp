#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::Initialize(SharedState* pSharedCtx) {
    /* Init shared context. */
    m_pSharedCtx = pSharedCtx;

    /* Initialize memory manager. */
    m_MemMgr.Initialize(m_pSharedCtx->GetMemController(), m_pSharedCtx->GetMemMonitor());

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
