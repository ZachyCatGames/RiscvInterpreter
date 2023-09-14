#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmuTest/cpu/test_HartTestCase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>
#include <limits>

namespace riscv {
namespace test {

namespace {

class TestInstAUIPC : public HartSingleInstTestBase<TestInstAUIPC> {
public:
    constexpr TestInstAUIPC(std::string_view name, cpu::Opcode op, RegPairT rd, Word imm, NativeWord initialPC) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeUTypeInstruction(op, GetPairId(rd), imm)), name),
        m_ExpectedRd(rd),
        m_Offset(imm),
        m_InitialPC(initialPC) {}
private:
    friend class HartSingleInstTestBase<TestInstAUIPC>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Write PC. */
        pSys->WritePC(m_InitialPC);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check that rd is what we expect. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
    Word m_Offset;
    NativeWord m_InitialPC;
}; // class TestInstAUIPC

constexpr TestFramework g_TestRunner{
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test AUIPC with an immediate of zero. */
        TestInstAUIPC{
            "AUIPC_ZeroImm",
            cpu::Opcode::AUIPC,
            { 1, 0x1000 },
            0,
            0x1000
        },

        /* Test AUIPC with a positive immediate. */
        TestInstAUIPC{
            "AUIPC_PosImm",
            cpu::Opcode::AUIPC,
            { 1, 0x1000 + (1 << 15) },
            1 << 15,
            0x1000
        },

        /* Test AUIPC with a negative immediate. */
        TestInstAUIPC{
            "AUIPC_NegImm",
            cpu::Opcode::AUIPC,
            { 1, 0x20000 - 0x10000 },
            static_cast<Word>(-0x10000),
            0x20000
        },

        /* Test AUIPC with overflow. */
        TestInstAUIPC{
            "AUIPC_Overflow",
            cpu::Opcode::AUIPC,
            { 1, std::numeric_limits<NativeWord>::max() + 0x10000 },
            0x10000,
            std::numeric_limits<NativeWord>::max()
        },

        /* Test AUIPC with underflow. */
        TestInstAUIPC{
            "AUIPC_Underflow",
            cpu::Opcode::AUIPC,
            { 1, 0 - 0x10000 },
            static_cast<Word>(-0x10000),
            0
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
