#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>
#include <RiscvEmu/cpu/detail/cpu_SharedHartContext.h>
#include <RiscvEmu/cpu/cpu_PrivilageLevel.h>
#include <RiscvEmu/cpu/cpu_Result.h>
#include <cassert>

namespace riscv {
namespace cpu {

class Hart {
public:
    Hart(detail::SharedHartContext* pSharedCtx) :
        m_pSharedCtx(pSharedCtx) {}

    Result ExecuteInstruction(Instruction inst);

    void WritePC(NativeWord addr) noexcept { m_PC = addr; }
    void WriteGPR(int index, NativeWord value) noexcept {
        assert(index < c_NumGPR && index >= 0);
        m_GPR[index] = value;
    }

    NativeWord ReadPC() const noexcept { return m_PC; }
    NativeWord ReadGPR(int index) const noexcept {
        assert(index < c_NumGPR && index >= 0);
        return m_GPR[index];
    }

    Result ExecuteAtPc();

    Result Reset();
private:
    class InstructionRunner;

private:
    Result SignalBranch(Address offset) {
        m_PC += offset;
        return ResultSuccess();
    }

    Result SignalJump(Address addr) {
        m_PC = addr;
        return ResultSuccess();
    }
private:
    using MemCtlT = mem::MemoryController;

    template<typename T>
    Result MemoryReadImpl(auto func, T* pOut, Address addr) {
        /* If running in Machine mode perform a direct physical memory read. */
        if(m_CurPrivLevel == PrivilageLevel::Machine) {
            return (*m_pSharedCtx->GetMemoryController().*func)(pOut, addr);
        }

        /* Otherwise obtain a physical address from the MMU. */
        /* TODO: Supervisor. */
        /* TODO: MMU. */
        return ResultNotImplemented();
    }

    template<typename T>
    Result MemoryWriteImpl(auto func, T in, Address addr) {
        /* If running in machine mode perform a direct physical memory write. */
        if(m_CurPrivLevel == PrivilageLevel::Machine) {
            return (*m_pSharedCtx->GetMemoryController().*func)(in, addr);
        }

        /* Otherwise obtain a physical address from the MMU. */
        /* TODO: Supervisor. */
        /* TODO: MMU. */
        return ResultNotImplemented();
    }

    Result MemoryReadByte(Byte* pOut, Address addr) { 
        return this->MemoryReadImpl(&MemCtlT::ReadByte, pOut, addr); 
    }
    Result MemoryReadHWord(HWord* pOut, Address addr) {
        return this->MemoryReadImpl(&MemCtlT::ReadHWord, pOut, addr);
    }
    Result MemoryReadWord(Word* pOut, Address addr) {
        return this->MemoryReadImpl(&MemCtlT::ReadWord, pOut, addr);
    }
    Result MemoryReadDWord(DWord* pOut, Address addr) {
        return this->MemoryReadImpl(&MemCtlT::ReadDWord, pOut, addr);
    }
    Result MemoryWriteByte(Byte in, Address addr) {
        return this->MemoryWriteImpl(&MemCtlT::WriteByte, in, addr);
    }
    Result MemoryWriteHWord(HWord in, Address addr) {
        return this->MemoryWriteImpl(&MemCtlT::WriteHWord, in, addr);
    }
    Result MemoryWriteWord(Word in, Address addr) {
        return this->MemoryWriteImpl(&MemCtlT::WriteWord, in, addr);
    }
    Result MemoryWriteDWord(DWord in, Address addr) {
        return this->MemoryWriteImpl(&MemCtlT::WriteDWord, in, addr);
    }

    Result FetchInstruction(Instruction* pOut, Address addr) {
        Word inst = 0;
        Result res;

        /* If running in machine mode perform a direct physical memory write. */
        if(m_CurPrivLevel == PrivilageLevel::Machine) {
            res = m_pSharedCtx->GetMemoryController()->ReadWord(&inst, addr);
            *pOut = Instruction(inst);
            return res;
        }

        /* Otherwise obtain a physical address from the MMU. */
        /* TODO: Supervisor. */
        /* TODO: MMU. */
        return ResultNotImplemented();
    }

private:
    static constexpr auto c_NumGPR = cfg::cpu::EnableIsaRV32E ? 16 : 32;
    NativeWord m_PC;
    NativeWord m_GPR[c_NumGPR];

    PrivilageLevel m_CurPrivLevel;

    detail::SharedHartContext* const m_pSharedCtx;
}; // class Hart

} // namespace cpu
} // namespace riscv
