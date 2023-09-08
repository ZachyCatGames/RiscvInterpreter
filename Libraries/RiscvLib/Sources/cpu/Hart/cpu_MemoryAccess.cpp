#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

template<typename T>
Result Hart::MemoryReadImpl(auto func, T* pOut, Address addr) {
    /* If running in Machine mode perform a direct physical memory read. */
    if(m_CurPrivLevel == PrivilageLevel::Machine) {
        return (*m_SharedCtx.GetMemController().*func)(pOut, addr);
    }

    /* Otherwise obtain a physical address from the MMU. */
    /* TODO: Supervisor. */
    /* TODO: MMU. */
    return ResultNotImplemented();
}

template<typename T>
Result Hart::MemoryWriteImpl(auto func, T in, Address addr) {
    /* If running in machine mode perform a direct physical memory write. */
    if(m_CurPrivLevel == PrivilageLevel::Machine) {
        return (*m_SharedCtx.GetMemController().*func)(in, addr);
    }

    /* Otherwise obtain a physical address from the MMU. */
    /* TODO: Supervisor. */
    /* TODO: MMU. */
    return ResultNotImplemented();
}

Result Hart::MemoryReadByte(Byte* pOut, Address addr) { 
    return this->MemoryReadImpl(&MemCtlT::ReadByte, pOut, addr); 
}
Result Hart::MemoryReadHWord(HWord* pOut, Address addr) {
    return this->MemoryReadImpl(&MemCtlT::ReadHWord, pOut, addr);
}
Result Hart::MemoryReadWord(Word* pOut, Address addr) {
    return this->MemoryReadImpl(&MemCtlT::ReadWord, pOut, addr);
}
Result Hart::MemoryReadDWord(DWord* pOut, Address addr) {
    return this->MemoryReadImpl(&MemCtlT::ReadDWord, pOut, addr);
}
Result Hart::MemoryWriteByte(Byte in, Address addr) {
    return this->MemoryWriteImpl(&MemCtlT::WriteByte, in, addr);
}
Result Hart::MemoryWriteHWord(HWord in, Address addr) {
    return this->MemoryWriteImpl(&MemCtlT::WriteHWord, in, addr);
}
Result Hart::MemoryWriteWord(Word in, Address addr) {
    return this->MemoryWriteImpl(&MemCtlT::WriteWord, in, addr);
}
Result Hart::MemoryWriteDWord(DWord in, Address addr) {
    return this->MemoryWriteImpl(&MemCtlT::WriteDWord, in, addr);
}

Result Hart::FetchInstruction(Instruction* pOut, Address addr) {
    Word inst = 0;
    Result res;

    /* If running in machine mode perform a direct physical memory write. */
    if(m_CurPrivLevel == PrivilageLevel::Machine) {
        res = m_SharedCtx.GetMemController()->ReadWord(&inst, addr);
        *pOut = Instruction(inst);
        return res;
    }

    /* Otherwise obtain a physical address from the MMU. */
    /* TODO: Supervisor. */
    /* TODO: MMU. */
    return ResultNotImplemented();
}

} // namespace cpu
} // namespace riscv
