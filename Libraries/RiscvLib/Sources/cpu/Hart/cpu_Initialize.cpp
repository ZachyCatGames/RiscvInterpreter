#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::Initialize(SharedContext sharedCtx) {
    /* Init shared context. */
    m_SharedCtx = sharedCtx;

    /* Initialize memory manager. */
    m_MemMgr.Initialize(m_SharedCtx.GetMemController());

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
