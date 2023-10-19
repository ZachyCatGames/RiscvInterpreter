#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::Reset() {
    /* Reset privilage level to machine. */
    m_CurPrivLevel = PrivilageLevel::Machine;

    /* Initialize cycle counter to zero. */
    m_CycleCount = 0;

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
