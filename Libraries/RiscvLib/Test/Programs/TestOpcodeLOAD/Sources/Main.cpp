#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>

namespace riscv {
namespace test {

namespace {

class LoadInstTest : public HartSingleInstTestBase<LoadInstTest> {
public:
    using AddrPairT = std::pair<Address, NativeWord>;
    constexpr LoadInstTest(std::string_view name, cpu::Opcode op, cpu::Funct3 f3, RegPairT rd, RegPairT rs1, Word imm, AddrPairT memVal) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, f3, GetPairId(rd), GetPairId(rs1), imm)), name),
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

constexpr TestFramework g_TestRunner(
    &HartTestSystem::DefaultReset,

    std::tuple {
        /* Test LB with an offset of 0. */
        LoadInstTest{
            "LB_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, 0x11 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LB with a positive offset. */
        LoadInstTest{
            "LB_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, 0x11 },
            { 31, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11 }
        },

        /* Test LB with a negative offset. */
        LoadInstTest{
            "LB_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, 0x11 },
            { 31, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LB with a signed value. */
        LoadInstTest{
            "LB_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, -69 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LH with an offset of 0. */
        LoadInstTest{
            "LH_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, 0x1122 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LH with a positive offset. */
        LoadInstTest{
            "LH_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, 0x1122 },
            { 31, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122 }
        },

        /* Test LH with a negative offset. */
        LoadInstTest{
            "LH_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, 0x1122 },
            { 31, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LH with a signed value. */
        LoadInstTest{
            "LH_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, -69 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LW with an offset of 0. */
        LoadInstTest{
            "LW_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, 0x11223344 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LW with a positive offset. */
        LoadInstTest{
            "LW_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, 0x11223344 },
            { 31, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11223344 }
        },

        /* Test LW with a negative offset. */
        LoadInstTest{
            "LW_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, 0x11223344 },
            { 31, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LW with a signed value. */
        LoadInstTest{
            "LW_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, -69 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LBU with an offset of 0. */
        LoadInstTest{
            "LBU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x11 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LBU with a positive offset. */
        LoadInstTest{
            "LBU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x11 },
            { 31, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11 }
        },

        /* Test LBU with a negative offset. */
        LoadInstTest{
            "LBU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x11 },
            { 31, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LBU with a signed value. */
        LoadInstTest{
            "LBU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x80 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x80 }
        },

        /* Test LHU with an offset of 0. */
        LoadInstTest{
            "LHU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x1122 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LHU with a positive offset. */
        LoadInstTest{
            "LHU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x1122 },
            { 31, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122 }
        },

        /* Test LHU with a negative offset. */
        LoadInstTest{
            "LHU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x1122 },
            { 31, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LHU with a signed value. */
        LoadInstTest{
            "LHU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x8000 },
            { 31, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x8000 }
        },
    }
);

} // namespace

extern const std::string_view ProgramName = "TestOpcodeLOAD";

extern Result Main([[maybe_unused]] Args args) {
    static HartTestSystem sys;

    sys.Initialize();
    g_TestRunner.RunAll(&sys);

    return ResultSuccess();
}

} // namespace test
} // namespace riscv

