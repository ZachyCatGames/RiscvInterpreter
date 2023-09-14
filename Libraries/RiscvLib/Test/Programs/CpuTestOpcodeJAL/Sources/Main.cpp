#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmuTest/cpu/test_HartTestCase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>

namespace riscv {
namespace test {

namespace {

class TestInstJAL : public HartSingleInstTestBase<TestInstJAL> {
public:
    constexpr TestInstJAL(std::string_view name, cpu::Opcode op, RegPairT rd, Word imm, NativeWord initialPC, NativeWord expectedPC) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeJTypeInstruction(op, GetPairId(rd), imm)), name),
        m_ExpectedRd(rd),
        m_InitialPC(initialPC),
        m_ExpectedPC(expectedPC) {}
private:
    friend class HartSingleInstTestBase<TestInstJAL>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Write PC. */
        pSys->WritePC(m_InitialPC);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check that pc is what we expect. */
        if(pSys->ReadPC() != m_ExpectedPC) {
            return ResultRegValMismatch();
        }

        /* Check that rd is what we expect. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
    NativeWord m_InitialPC;
    NativeWord m_ExpectedPC;
}; // class TestInstJAL

constexpr TestFramework g_TestRunner{
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test JAL with no offset. */
        TestInstJAL{
            "JAL_ZeroImm",
            cpu::Opcode::JAL,
            { 1, 0x1004 },
            0,
            0x1000,
            0x1000
        },

        /* Test JAL with a positive offset. */
        TestInstJAL{
            "JAL_PosImm",
            cpu::Opcode::JAL,
            { 1, 0x1004 },
            0x100,
            0x1000,
            0x1100
        },

        /* Test JAL with a negative offset. */
        TestInstJAL{
            "JAL_NegImm",
            cpu::Opcode::JAL,
            { 1, 0x1004 },
            static_cast<Word>(-0x100),
            0x1000,
            0xF00
        },

        /* Test JAL with overflow on PC. */
        TestInstJAL{
            "JAL_Overflow",
            cpu::Opcode::JAL,
            { 1, std::numeric_limits<NativeWord>::max() - 3 },
            0x8,
            std::numeric_limits<NativeWord>::max() - 7,
            0x0
        },

        /* Test JAL with underflow on PC. */
        TestInstJAL{
            "JAL_Underflow",
            cpu::Opcode::JAL,
            { 1, 0x8 },
            static_cast<Word>(-0x8),
            0x4,
            static_cast<NativeWord>(4 - 8)
        },

        /* Test JAL with overflow on rd. */
        TestInstJAL{
            "JAL_OverflowRd",
            cpu::Opcode::JAL,
            { 1, 0 },
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
