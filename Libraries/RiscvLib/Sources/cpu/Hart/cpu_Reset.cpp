#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::Reset() {
    /* Reset privilage level to machine. */
    m_CurPrivLevel = PrivilageLevel::Machine;

    /* Initialize time. */
    m_ClkTime.Initialize();

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
