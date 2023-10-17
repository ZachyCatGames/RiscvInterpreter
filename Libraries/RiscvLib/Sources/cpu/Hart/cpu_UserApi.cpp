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
    return m_MemMgr.ReadByte(pOut, addr, PrivilageLevel::Machine);
}

Result Hart::MappedReadHWord(HWord* pOut, Address addr) {
    return m_MemMgr.ReadHWord(pOut, addr, PrivilageLevel::Machine);
}

Result Hart::MappedReadWord(Word* pOut, Address addr) {
    return m_MemMgr.ReadWord(pOut, addr, PrivilageLevel::Machine);
}

Result Hart::MappedReadDWord(DWord* pOut, Address addr) {
    return m_MemMgr.ReadDWord(pOut, addr, PrivilageLevel::Machine);
}

Result Hart::MappedWriteByte(Byte in, Address addr) {
    return m_MemMgr.WriteByte(in, addr, PrivilageLevel::Machine);
}

Result Hart::MappedWriteHWord(HWord in, Address addr) {
    return m_MemMgr.WriteHWord(in, addr, PrivilageLevel::Machine);
}

Result Hart::MappedWriteWord(Word in, Address addr) {
    return m_MemMgr.WriteWord(in, addr, PrivilageLevel::Machine);
}

Result Hart::MappedWriteDWord(DWord in, Address addr) {
    return m_MemMgr.WriteDWord(in, addr, PrivilageLevel::Machine);
}

} // namespace cpu
} // namespace riscv
