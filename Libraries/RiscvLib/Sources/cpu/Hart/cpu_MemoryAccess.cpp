#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::FetchInstruction(Instruction* pOut, Address addr) {
    Word inst = 0;
    Result res = m_MemMgr.InstFetch(&inst, addr, m_CurPrivLevel);
    *pOut = Instruction(inst);
    return res;
}

} // namespace cpu
} // namespace riscv
