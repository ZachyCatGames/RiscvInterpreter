#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::MemReadByte(Byte* pOut, Address addr) {
    return m_MemMgr.ReadByte(pOut, addr, m_CurPrivLevel);
}
Result Hart::MemReadHWord(HWord* pOut, Address addr) {
    return m_MemMgr.ReadHWord(pOut, addr, m_CurPrivLevel);
}
Result Hart::MemReadWord(Word* pOut, Address addr) {
    return m_MemMgr.ReadWord(pOut, addr, m_CurPrivLevel);
}
Result Hart::MemReadDWord(DWord* pOut, Address addr) {
    return m_MemMgr.ReadDWord(pOut, addr, m_CurPrivLevel);
}
Result Hart::MemWriteByte(Byte in, Address addr) {
    return m_MemMgr.WriteByte(in, addr, m_CurPrivLevel);
}
Result Hart::MemWriteHWord(HWord in, Address addr) {
    return m_MemMgr.WriteHWord(in, addr, m_CurPrivLevel);
}
Result Hart::MemWriteWord(Word in, Address addr) {
    return m_MemMgr.WriteWord(in, addr, m_CurPrivLevel);
}
Result Hart::MemWriteDWord(DWord in, Address addr) {
    return m_MemMgr.WriteDWord(in, addr, m_CurPrivLevel);
}

Result Hart::FetchInstruction(Instruction* pOut, Address addr) {
    Word inst = 0;
    Result res = m_MemMgr.InstFetch(&inst, addr, m_CurPrivLevel);
    *pOut = Instruction(inst);
}

} // namespace cpu
} // namespace riscv
