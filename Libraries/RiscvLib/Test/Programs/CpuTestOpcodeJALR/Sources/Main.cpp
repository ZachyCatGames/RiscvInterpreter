#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>

namespace riscv {
namespace test {

namespace {

class TestInstJALR : public HartSingleInstTestBase<TestInstJALR> {
public:
    constexpr TestInstJALR(std::string_view name, cpu::Opcode op, cpu::Funct3 f3, RegPairT rd, RegPairT rs1, Word imm, NativeWord initialPC, NativeWord expectedPC) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, f3, GetPairId(rd), GetPairId(rs1), imm)), name),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1),
        m_InitialPC(initialPC),
        m_ExpectedPC(expectedPC) {}
private:
    friend class HartSingleInstTestBase<TestInstJALR>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Write rs1. */
        pSys->WriteGPR(m_InitialRs1);

        /* Write PC. */
        pSys->WritePC(m_InitialPC);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check that PC is what we expect. */
        if(pSys->ReadPC() != m_ExpectedPC) {
            return ResultRegValMismatch();
        }

        /* Check rd value. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
    RegPairT m_InitialRs1;
    NativeWord m_InitialPC;
    NativeWord m_ExpectedPC;
}; // class TestInstJALR

constexpr TestFramework g_TestRunner{
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test JALR with no offset. */
        TestInstJALR{
            "JALR_NoImm",
            cpu::Opcode::JALR,
            cpu::Funct3::JALR,
            { 1, 0x2004 },
            { 15, 0x1000 },
            0,
            0x2000,
            0x1000
        },

        /* Test JALR with a positive offset. */
        TestInstJALR{
            "JALR_PosImm",
            cpu::Opcode::JALR,
            cpu::Funct3::JALR,
            { 1, 0x2004 },
            { 15, 0x1000 },
            0x100,
            0x2000,
            0x1100
        },

        /* Test JALR with a negative offset. */
        TestInstJALR{
            "JALR_NegImm",
            cpu::Opcode::JALR,
            cpu::Funct3::JALR,
            { 1, 0x2004 },
            { 15, 0x1000 },
            static_cast<Word>(-0x100),
            0x2000,
            0xF00
        },

        /* Test JALR with overflow on PC. */
        TestInstJALR{
            "JALR_Overflow",
            cpu::Opcode::JALR,
            cpu::Funct3::JALR,
            { 1, 0x2004 },
            { 15, std::numeric_limits<NativeWord>::max() - 7 },
            8,
            0x2000,
            0
        },

        /* Test JALR with underflow on PC. */
        TestInstJALR{
            "JALR_Underflow",
            cpu::Opcode::JALR,
            cpu::Funct3::JALR,
            { 1, 0x2004 },
            { 15, 0 },
            static_cast<Word>(-4),
            0x2000,
            static_cast<NativeWord>(-4)
        },

        /* Test JALR with overflow on rd. */
        TestInstJALR{
            "JALR_OverflowRd",
            cpu::Opcode::JALR,
            cpu::Funct3::JALR,
            { 1, 0 },
            { 15, std::numeric_limits<NativeWord>::max() - 3 },
            static_cast<Word>(-0x100),
            std::numeric_limits<NativeWord>::max() - 3,
            std::numeric_limits<NativeWord>::max() - 3 - 0x100
        }
    }
};

} // namespace

TestResults Main([[maybe_unused]] Args args) {
    HartTestSystem sys;

    sys.Initialize();

    return g_TestRunner.RunAll(&sys);
}

} // namespace test
} // namespace riscv
