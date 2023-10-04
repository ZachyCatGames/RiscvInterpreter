#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_PrivilageLevel.h>
#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/cpu_TrapCode.h>
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

    constexpr void WritePC(NativeWord addr) noexcept { m_PC = addr; }
    constexpr void WriteGPR(int index, NativeWord value) noexcept {
        assert(index < NumGPR && index >= 0);
        m_GPR[index] = value;
    }

    constexpr NativeWord ReadPC() const noexcept { return m_PC; }
    constexpr NativeWord ReadGPR(int index) const noexcept {
        assert(index < NumGPR && index >= 0);
        return m_GPR[index];
    }

    Result ExecuteAtPc();

    Result Reset();
private:
    class InstructionRunner;
private:
    constexpr Result SignalBranch(Address offset) {
        /* TODO: Check alignment, throw exception if misaligned. */

        m_PC += offset;
        return ResultSuccess();
    }

    constexpr Result SignalJump(Address addr) {
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
    Result TriggerTrap(TrapCode code);
private:
    constexpr NativeWord ReadPrivPC(PrivilageLevel level) const noexcept {
        return m_PrivPC[static_cast<int>(level)];
    }

    constexpr void WritePrivPC(NativeWord val, PrivilageLevel level) noexcept {
        m_PrivPC[static_cast<int>(level)] = val;
    }

    constexpr void SwapPrivPC(PrivilageLevel newLevel) noexcept {
        /* Write our current PC to the slot for our current priv level. */
        this->WritePrivPC(m_PC, m_CurPrivLevel);

        /* Load PC for newLevel. */
        m_PC = this->ReadPrivPC(newLevel);
    }
private:
    constexpr NativeWord GetExcVectAddr(PrivilageLevel level) const noexcept {
        return m_TrapVectAddr[static_cast<int>(level)];
    }

    constexpr NativeWord GetActiveExcVectAddr() const noexcept {
        return this->GetExcVectAddr(m_CurPrivLevel);
    }
public:
    static constexpr auto NumGPR = cfg::cpu::EnableIsaRV32E ? 16 : 32;
private:
    NativeWord m_PC;
    NativeWord m_GPR[NumGPR];

    PrivilageLevel m_CurPrivLevel;

    NativeWord m_HartId;

    /** Cycle & retired inst count. */
    DWord m_CycleCount;

    /** Stored PC for each privilage level. */
    NativeWord m_PrivPC[4];

    /** Trap vector address for each privilage level. */
    NativeWord m_TrapVectAddr[4];

    SharedContext m_SharedCtx;
}; // class Hart

} // namespace cpu
} // namespace riscv
