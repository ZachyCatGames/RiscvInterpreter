#include <RiscvEmu/riscv_System.h>

namespace riscv {

Result System::Initialize() {
    /* Initialize Hart(s). */
    Hart::SharedContext hartShared(&m_MemCtlr);
    Result res = m_Hart.Initialize(hartShared);
    if(res.IsFailure()) {
        return res;
    }

    /* Reset Hart(s). */
    res = m_Hart.Reset();
    if(res.IsFailure()) {
        return res;
    }

    return ResultSuccess();
}

} // namespace riscv
