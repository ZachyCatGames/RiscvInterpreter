#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_CsrId.h>
#include <RiscvEmu/cpu/cpu_Types.h>
#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/cpu_TrapCode.h>
#include <RiscvEmu/cpu/cpu_InstructionFormat.h>
#include <RiscvEmu/cpu/detail/cpu_MemoryManager.h>
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
    /** Initialize the Hart. */
    Result Initialize(SharedContext pSharedCtx);
public:
    /** Read the instruction currently at PC. */
    Result FetchInstAtPc(Instruction* pOut);

    /** Execute an instruction. */
    Result ExecuteInst(Instruction inst);

    /** Execute the instruction at PC (atomic ReadInstructionAtPc + ExecuteInstruction) */
    Result ExecuteInstAtPc();

    /** Write the PC register. */
    constexpr void WritePC(NativeWord addr) noexcept { m_PC = addr; }

    /** Write a general purpose register. */
    constexpr void WriteGPR(int index, NativeWord value) noexcept {
        assert(index < NumGPR && index >= 0);
        m_GPR[index] = value;
    }

    /** Read the PC register. */
    constexpr NativeWord ReadPC() const noexcept { return m_PC; }

    /** Read a general purpose register. */
    constexpr NativeWord ReadGPR(int index) const noexcept {
        assert(index < NumGPR && index >= 0);
        return m_GPR[index];
    }

    /** Write a control/status register. */
    Result WriteCSR(CsrId id, NativeWord value);

    /** Read a control/status register. */
    Result ReadCSR(CsrId id, NativeWord* pOut);

    /** Read and write a control/status register. */
    Result ReadWriteCSR(CsrId id, NativeWord* pOut, NativeWord in);

    /** Read mapped Byte with highest privilage. */
    Result MappedReadByte(Byte* pOut, Address addr);

    /** Read mapped HWord with highest privilage. */
    Result MappedReadHWord(HWord* pOut, Address addr);

    /** Read mapped Word with highest privilage. */
    Result MappedReadWord(Word* pOut, Address addr);

    /** Read mapped DWord with highest privilage. */
    Result MappedReadDWord(DWord* pOut, Address addr);

    /** Write mapped Byte with highest privilage. */
    Result MappedWriteByte(Byte in, Address addr);

    /** Write mapped HWord with highest privilage. */
    Result MappedWriteHWord(HWord in, Address addr);

    /** Write mapped Word with highest privilage. */
    Result MappedWriteWord(Word in, Address addr);

    /** Write mapped DWord with highest privilage. */
    Result MappedWriteDWord(DWord in, Address addr);

    Result Reset();
private:
    class InstructionRunner;
    Result ExecuteInstructionImpl(Instruction inst);
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
    Result MemReadByte(Byte* pOut, Address addr);
    Result MemReadHWord(HWord* pOut, Address addr);
    Result MemReadWord(Word* pOut, Address addr);
    Result MemReadDWord(DWord* pOut, Address addr);
    Result MemWriteByte(Byte in, Address addr);
    Result MemWriteHWord(HWord in, Address addr);
    Result MemWriteWord(Word in, Address addr);
    Result MemWriteDWord(DWord in, Address addr);

    Result FetchInstruction(Instruction* pOut, Address addr);
private:
    Result TriggerTrap(TrapCode code);
private:
    using CsrMakeValFunc = NativeWord(*)(NativeWord curVal, NativeWord writeVal);
    Result ReadWriteCSRImpl(CsrId id, NativeWord* pOut, NativeWord writeVal, CsrMakeValFunc makeValFunc);

    using RwmCSRReadFunc = Result(Hart::*)(NativeWord*);
    using RwmCSRWriteFunc = Result(Hart::*)(NativeWord);
    Result RwmCSRImpl(NativeWord* pOut, NativeWord writeVal, RwmCSRReadFunc readFunc, RwmCSRWriteFunc writeFunc, CsrMakeValFunc makeValFunc);

    Result CSRReadSatp(NativeWord* pOut);
    Result CSRWriteSatp(NativeWord val);
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

    detail::MemoryManager m_MemMgr;

    SharedContext m_SharedCtx;
}; // class Hart

} // namespace cpu
} // namespace riscv
