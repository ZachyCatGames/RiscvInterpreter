#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>

namespace riscv {
namespace test {

namespace {

class TestInstLUI : public HartSingleInstTestBase<TestInstLUI> {
public:
    constexpr TestInstLUI(std::string_view name, cpu::Opcode op, RegPairT rd, Word imm) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeUTypeInstruction(op, GetPairId(rd), imm)), name),
        m_ExpectedRd(rd) {}
private:
    friend class HartSingleInstTestBase<TestInstLUI>;
    Result Initialize([[maybe_unused]] HartTestSystem* pSys) const {
        /* Do nothing. */
        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check that rd is what we expect. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
};

constexpr TestFramework g_TestRunner{
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test LUI with a immediate of zero. */
        TestInstLUI{
            "LUI_ZeroImm",
            cpu::Opcode::LUI,
            { 1, 0 },
            0
        },

        /* Test LUI with a positive immediate. */
        TestInstLUI{
            "LUI_PosImm",
            cpu::Opcode::LUI,
            { 1, 0x10000000 },
            0x10000000
        },

        /* Test LUI with a negative immediate. */
        TestInstLUI{
            "LUI_NegImm",
            cpu::Opcode::LUI,
            { 1, -0x100000 },
            static_cast<Word>(-0x100000)
        }
    }
};

} // namespace

extern constexpr std::string_view ProgramName = "CpuTestOpcodeLUI";

Result Main([[maybe_unused]] Args args) {
    HartTestSystem sys;

    sys.Initialize();

    return g_TestRunner.RunAll(&sys);
}

} // namespace test
} // namespace riscv
