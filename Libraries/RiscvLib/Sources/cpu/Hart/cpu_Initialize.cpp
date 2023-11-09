#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/diag.h>

namespace riscv {
namespace cpu {

Result Hart::Initialize(SharedState* pSharedCtx, Word hartId) {
    /* Assert shared context isn't null. */
    diag::AssertNotNull(pSharedCtx);

    /* Assign hart id. */
    m_HartId = hartId;

    /* Init shared context. */
    m_pSharedCtx = pSharedCtx;

    /* Initialize memory manager. */
    m_MemMgr.Initialize(m_pSharedCtx->GetMemController());

    /* Initialize memory monitor context. */
    m_MemMonitorCtx = m_pSharedCtx->GetMemMonitor()->GetContext(m_HartId);

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
