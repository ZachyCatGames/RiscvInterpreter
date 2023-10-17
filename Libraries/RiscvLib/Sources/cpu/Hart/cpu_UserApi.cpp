#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::FetchInstAtPc(Instruction* pOut) {
    return this->FetchInstruction(pOut, m_PC);
}

Result Hart::ExecuteInst(Instruction inst) {
    return this->ExecuteInstructionImpl(inst);
}

Result Hart::ExecuteInstAtPc() {
    Instruction inst(0);

    Result res = this->FetchInstAtPc(&inst);
    if(res.IsFailure()) {
        return res;
    }

    return this->ExecuteInst(inst);
}

Result Hart::WriteCSR([[maybe_unused]] int index, [[maybe_unused]] NativeWord value) {
    return ResultNotImplemented();
}

Result Hart::ReadCSR([[maybe_unused]] int index, [[maybe_unused]] NativeWord* pOut) {
    return ResultNotImplemented();
}

Result Hart::MappedReadByte(Byte* pOut, Address addr) {
    return m_MemMgr.MappedReadByte(pOut, addr);
}

Result Hart::MappedReadHWord(HWord* pOut, Address addr) {
    return m_MemMgr.MappedReadHWord(pOut, addr);
}

Result Hart::MappedReadWord(Word* pOut, Address addr) {
    return m_MemMgr.MappedReadWord(pOut, addr);
}

Result Hart::MappedReadDWord(DWord* pOut, Address addr) {
    return m_MemMgr.MappedReadDWord(pOut, addr);
}

Result Hart::MappedWriteByte(Byte in, Address addr) {
    return m_MemMgr.MappedWriteByte(in, addr);
}

Result Hart::MappedWriteHWord(HWord in, Address addr) {
    return m_MemMgr.MappedWriteHWord(in, addr);
}

Result Hart::MappedWriteWord(Word in, Address addr) {
    return m_MemMgr.MappedWriteWord(in, addr);
}

Result Hart::MappedWriteDWord(DWord in, Address addr) {
    return m_MemMgr.MappedWriteDWord(in, addr);
}

} // namespace cpu
} // namespace riscv
