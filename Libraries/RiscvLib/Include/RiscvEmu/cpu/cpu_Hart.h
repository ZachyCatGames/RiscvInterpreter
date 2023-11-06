#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_CsrFormat.h>
#include <RiscvEmu/cpu/cpu_CsrId.h>
#include <RiscvEmu/cpu/cpu_Types.h>
#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/cpu_TrapCode.h>
#include <RiscvEmu/cpu/cpu_InstructionFormat.h>
#include <RiscvEmu/cpu/detail/cpu_ClkTime.h>
#include <RiscvEmu/cpu/detail/cpu_MemoryManager.h>
#include <RiscvEmu/cpu/detail/cpu_MemoryMonitor.h>
#include <RiscvEmu/mem/mem_MemoryController.h>
#include <cassert>

namespace riscv {
namespace cpu {

class Hart {
public:
    class SharedState {
    public:
        void Initialize(std::size_t hartCount, mem::MemoryController* pMemCtlr) noexcept;

        mem::MemoryController* GetMemController() noexcept;

        detail::MemoryMonitor* GetMemMonitor() noexcept;
    private:
        std::size_t m_HartCount;
        mem::MemoryController* m_pMemCtlr;
        detail::MemoryMonitor m_MemMonitor;
    }; // SharedState
public:
    /** Initialize the Hart. */
    Result Initialize(SharedState* pSharedCtx, std::size_t hartId);
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
    Result ReadWriteCSRImpl(CsrId id, NativeWord* pOut, NativeWord writeVal = 0, CsrMakeValFunc makeValFunc = nullptr);

    using RmwCSRReadFunc = Result(Hart::*)(NativeWord*);
    using RmwCSRWriteFunc = Result(Hart::*)(NativeWord);
    Result RmwCSRImpl(NativeWord* pOut, NativeWord writeVal, RmwCSRReadFunc readFunc, RmwCSRWriteFunc writeFunc, CsrMakeValFunc makeValFunc);

    Result CSRRead_sscratch(NativeWord* pOut);
    Result CSRWrite_sscratch(NativeWord val);

    Result CSRRead_satp(NativeWord* pOut);
    Result CSRWrite_satp(NativeWord val);

    Result CSRRead_misa(NativeWord* pOut);
    Result CSRWrite_misa(NativeWord in);

    Result CSRRead_mscratch(NativeWord* pOut);
    Result CSRWrite_mscratch(NativeWord in);

    Result CSRRead_mcycle(NativeWord* pOut);
    Result CSRWrite_mcycle(NativeWord in);
    Result CSRRead_mcycleh(NativeWord* pOut);
    Result CSRWrite_mcycleh(NativeWord in);

    Result CSRRead_minstret(NativeWord* pOut);
    Result CSRWrite_minstret(NativeWord in);
    Result CSRRead_minstreth(NativeWord* pOut);
    Result CSRWrite_minstreth(NativeWord in);

    Result CSRRead_cycle(NativeWord* pOut);
    Result CSRRead_cycleh(NativeWord* pOut);

    Result CSRRead_instret(NativeWord* pOut);
    Result CSRRead_instreth(NativeWord* pOut);
private:
    NativeWord& GetEPC() noexcept;
    NativeWord& GetTrapVecBase() noexcept;

    Result SetupTrap(TrapCode code);

public:
    static constexpr auto NumGPR = cfg::cpu::EnableIsaRV32E ? 16 : 32;
private:
    static constexpr csr::misa c_misaValue = []() {
        csr::misa misa;
        misa.SetMXL(cfg::cpu::EnableIsaRV64I ? 2 : 1);
        misa.SetI(true);
        misa.SetM(true);
        return misa;
    }();
private:
    NativeWord m_PC;
    NativeWord m_GPR[NumGPR];

    PrivilageLevel m_CurPrivLevel;

    std::size_t m_HartId;

    /** Cycle & retired inst count. */
    DWord m_CycleCount;

    /** EPC for each privilage level. */
    NativeWord m_EPC[4];

    /** tvec for each privilage level. */
    NativeWord m_TrapVecBase[4];

    /** Stored PC for each privilage level. */
    NativeWord m_PrivPC[4];

    /** Trap cause for each privilage level. */
    TrapCode m_TrapCause[4];

    /* Supervisor/Hypervisor/Machine scratch register. */
    NativeWord m_MachineScratch;
    //NativeWord m_HypervisorScratch;
    NativeWord m_SupervisorScratch;

    /* Memory manager. */
    detail::MemoryManager m_MemMgr;

    /* Memory monitor context for this hart. */
    detail::MemoryMonitor::Context m_MemMonitorCtx;

    SharedState* m_pSharedCtx;
}; // class Hart

} // namespace cpu
} // namespace riscv
