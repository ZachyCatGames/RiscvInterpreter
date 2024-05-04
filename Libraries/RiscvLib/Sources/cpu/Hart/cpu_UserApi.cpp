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

} // namespace cpu
} // namespace riscv
