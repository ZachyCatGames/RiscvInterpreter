#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_PrivilageLevel.h>
#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>
#include <RiscvEmu/mem/mem_MemoryController.h>
#include <cassert>

namespace riscv {
namespace cpu {

class Hart {
public:
    class SharedContext {
    public:
        constexpr SharedContext() noexcept = default;
        constexpr SharedContext(mem::MemoryController* pMemCtlr) noexcept :
            m_pMemCtlr(pMemCtlr) {}
        
        constexpr auto GetMemController() const noexcept { return m_pMemCtlr; }
    private:
        mem::MemoryController* m_pMemCtlr;
    }; // SharedContext
public:
    Result Initialize(SharedContext pSharedCtx);

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
    Result MemoryReadImpl(auto func, T* pOut, Address addr);

    template<typename T>
    Result MemoryWriteImpl(auto func, T in, Address addr);

public:
    Result MemoryReadByte(Byte* pOut, Address addr);
    Result MemoryReadHWord(HWord* pOut, Address addr);
    Result MemoryReadWord(Word* pOut, Address addr);
    Result MemoryReadDWord(DWord* pOut, Address addr);
    Result MemoryWriteByte(Byte in, Address addr);
    Result MemoryWriteHWord(HWord in, Address addr);
    Result MemoryWriteWord(Word in, Address addr);
    Result MemoryWriteDWord(DWord in, Address addr);

    Result FetchInstruction(Instruction* pOut, Address addr);

private:
    static constexpr auto c_NumGPR = cfg::cpu::EnableIsaRV32E ? 16 : 32;
    NativeWord m_PC;
    NativeWord m_GPR[c_NumGPR];

    PrivilageLevel m_CurPrivLevel;

    SharedContext m_SharedCtx;
}; // class Hart

} // namespace cpu
} // namespace riscv
