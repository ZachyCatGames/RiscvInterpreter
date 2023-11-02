#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

NativeWord& Hart::GetEPC() noexcept { return m_EPC[static_cast<int>(m_CurPrivLevel)]; }

NativeWord& Hart::GetTrapVecBase() noexcept { return m_TrapVecBase[static_cast<int>(m_CurPrivLevel)]; }

Result Hart::SetupTrap(TrapCode code) {
    /* Store PC in EPC staging. */
    m_EPC[0] = m_PC;

    /* Store trap code in Cause staging. */
    m_TrapCause[0] = code;
}

} // namespace cpu
} // namespace riscv
