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

Result Hart::WriteCSR(int index, NativeWord value) {
    return ResultNotImplemented();
}

Result Hart::ReadCSR(int index, NativeWord* pOut) {
    return ResultNotImplemented();
}

} // namespace cpu
} // namespace riscv
