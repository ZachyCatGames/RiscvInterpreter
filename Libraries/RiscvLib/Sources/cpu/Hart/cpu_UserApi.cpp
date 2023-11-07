#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/diag.h>

namespace riscv {
namespace cpu {

Result Hart::FetchInstAtPc(Instruction* pOut) {
    diag::AssertNotNull(pOut);
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

Result Hart::WriteCSR(CsrId id, NativeWord value) {
    auto makeValFunc = [](NativeWord, NativeWord writeVal) {
        return writeVal;
    };

    NativeWord tmp;
    return this->ReadWriteCSRImpl(id, &tmp, value, makeValFunc);

}

Result Hart::ReadCSR(CsrId id, NativeWord* pOut) {
    return this->ReadWriteCSRImpl(id, pOut);
}

Result Hart::ReadWriteCSR(CsrId id, NativeWord* pOut, NativeWord in) {
    auto makeValFunc = [](NativeWord, NativeWord writeVal) {
        return writeVal;
    };

    return this->ReadWriteCSRImpl(id, pOut, in, makeValFunc);
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
