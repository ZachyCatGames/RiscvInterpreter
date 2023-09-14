#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>

/* TODO: Load Access/Page faults. */

namespace riscv {
namespace test {

namespace {

class TestInstLOAD : public HartSingleInstTestBase<TestInstLOAD> {
public:
    using AddrPairT = std::pair<Address, NativeWord>;
    constexpr TestInstLOAD(std::string_view name, cpu::Opcode op, cpu::Funct3 f3, RegPairT rd, RegPairT rs1, Word imm, AddrPairT memVal) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, f3, GetPairId(rd), GetPairId(rs1), imm)), name),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1),
        m_MemVal(memVal) {}
private:
    friend class HartSingleInstTestBase<TestInstLOAD>;
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
}; // class TestInstLOAD

constexpr TestFramework g_TestRunner(
    &HartTestSystem::DefaultReset,

    std::tuple {
        /* Test LB with an offset of 0. */
        TestInstLOAD{
            "LB_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LB with a positive offset. */
        TestInstLOAD{
            "LB_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11 }
        },

        /* Test LB with a negative offset. */
        TestInstLOAD{
            "LB_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LB with a signed value. */
        TestInstLOAD{
            "LB_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LB,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LH with an offset of 0. */
        TestInstLOAD{
            "LH_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LH with a positive offset. */
        TestInstLOAD{
            "LH_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122 }
        },

        /* Test LH with a negative offset. */
        TestInstLOAD{
            "LH_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LH with a signed value. */
        TestInstLOAD{
            "LH_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LH,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LW with an offset of 0. */
        TestInstLOAD{
            "LW_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LW with a positive offset. */
        TestInstLOAD{
            "LW_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11223344 }
        },

        /* Test LW with a negative offset. */
        TestInstLOAD{
            "LW_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LW with a signed value. */
        TestInstLOAD{
            "LW_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LW,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LBU with an offset of 0. */
        TestInstLOAD{
            "LBU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LBU with a positive offset. */
        TestInstLOAD{
            "LBU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11 }
        },

        /* Test LBU with a negative offset. */
        TestInstLOAD{
            "LBU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LBU with a signed value. */
        TestInstLOAD{
            "LBU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LBU,
            { 1, 0x80 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x80 }
        },

        /* Test LHU with an offset of 0. */
        TestInstLOAD{
            "LHU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LHU with a positive offset. */
        TestInstLOAD{
            "LHU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122 }
        },

        /* Test LHU with a negative offset. */
        TestInstLOAD{
            "LHU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LHU with a signed value. */
        TestInstLOAD{
            "LHU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LHU,
            { 1, 0x8000 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x8000 }
        },
    }
);

} // namespace

TestResults Main([[maybe_unused]] Args args) {
    static HartTestSystem sys;

    sys.Initialize();

    return g_TestRunner.RunAll(&sys);
}

} // namespace test
} // namespace riscv

