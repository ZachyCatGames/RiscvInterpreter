#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/cpu/cpu_EncodeInstruction.h>

namespace riscv {
namespace test {

namespace {

class LoadInstTest : public HartSingleInstTestBase<LoadInstTest> {
public:
    using AddrPairT = std::pair<Address, NativeWord>;
    constexpr LoadInstTest(Result expectedRes, cpu::Opcode op, cpu::Funct3 f3, RegPairT rd, RegPairT rs1, Word imm, AddrPairT memVal) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, f3, GetPairId(rd), GetPairId(rs1), imm)), expectedRes),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1),
        m_MemVal(memVal) {}

private:
    friend class HartSingleInstTestBase<LoadInstTest>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Write our memVal to memory. */
        Result res = pSys->MemWriteDWord(static_cast<DWord>(std::get<1>(m_MemVal)), std::get<0>(m_MemVal));
        if(res.IsFailure()) {
            return res;
        }

        /* Write rs1. */
        pSys->WriteGPR(m_InitialRs1);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check rd value. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
    RegPairT m_InitialRs1;
    AddrPairT m_MemVal;
}; // class LoadInstTest

static HartTestSystem g_System;

static Result Reset(HartTestSystem* pSys) {
    return pSys->GetHart()->Reset();
}

static constexpr TestFramework g_TestRunner(
    &Reset,
    std::tuple {
        LoadInstTest{
            ResultSuccess(),
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 4, 0xAABBCCDD },
            { 4, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0xAABBCCDD }
        }
    }
);

} // namespace

Result Main([[maybe_unused]] Args args) {
    HartTestSystem sys;

    sys.Initialize();
    g_TestRunner.RunAll(&sys);

    return ResultSuccess();
}

} // namespace test
} // namespace riscv

