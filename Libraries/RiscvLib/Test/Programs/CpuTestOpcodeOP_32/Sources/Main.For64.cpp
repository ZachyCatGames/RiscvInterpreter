#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmuTest/cpu/test_HartTestCase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>
#include <limits>

namespace riscv {
namespace test {

namespace {

constexpr TestFramework g_TestRunner{
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test ADDW with both rs1 and rs2 as zero. */
        HartRTypeInstTest{
            "ADDW_ZeroRs1ZeroRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, 0 },
            { 1, 0 },
            { 2, 0 }
        },

        /* Test ADDW with rs1 and rs2 as positive values. */
        HartRTypeInstTest{
            "ADDW_PosRs1PosRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, 50 },
            { 1, 30 },
            { 2, 20 }
        },

        /* Test ADDW with rs1 and rs2 as negative values. */
        HartRTypeInstTest{
            "ADDW_NegRs1NegRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, -50 },
            { 1, -30 },
            { 2, -20 },
        },

        /* Test ADDW with positive rs1 and negative rs2. */
        HartRTypeInstTest{
            "ADDW_PosRs1NegRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, 0 },
            { 1, 30 },
            { 2, -30 },
        },

        /* Test ADDW with negative rs1 and positive rs2. */
        HartRTypeInstTest{
            "ADDW_NegRs1PosRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, -10 },
            { 1, -30 },
            { 2, 20 }
        },

        /* Test ADDW overflow. */
        HartRTypeInstTest{
            "ADDW_Overflow",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, 0 },
            { 1, std::numeric_limits<NativeWord>::max() },
            { 2, 1 },
        },

        /* Test ADDW underflow. */
        HartRTypeInstTest{
            "ADDW_Underflow",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, std::numeric_limits<NativeWord>::max() },
            { 1, 0 },
            { 2, -1 },
        },

        /* Test ADDW with rs1 greater than 2^31-1, upper bits should be ignored. */
        HartRTypeInstTest{
            "ADDW_IgnoreUpper",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::ADDW,
            { 15, 5 },
            { 1, (1ull << 48) },
            { 2, 5 }
        },

        /* Test SUBW with both rs1 and rs2 as zero. */
        HartRTypeInstTest{
            "SUBW_ZeroRs1ZeroRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, 0 },
            { 1, 0 },
            { 2, 0 }
        },

        /* Test SUBW with rs1 and rs2 as positive values. */
        HartRTypeInstTest{
            "SUBW_PosRs1PosRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, 10 },
            { 1, 30 },
            { 2, 20 }
        },

        /* Test SUBW with rs1 and rs2 as negative values. */
        HartRTypeInstTest{
            "SUBW_NegRs1NegRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, -10 },
            { 1, -30 },
            { 2, -20 },
        },

        /* Test SUBW with positive rs1 and negative rs2. */
        HartRTypeInstTest{
            "SUBW_PosRs1NegRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, 60 },
            { 1, 30 },
            { 2, -30 },
        },

        /* Test SUBW with negative rs1 and positive rs2. */
        HartRTypeInstTest{
            "SUBW_NegRs1PosRs2",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, -50 },
            { 1, -30 },
            { 2, 20 }
        },

        /* Test SUBW overflow. */
        HartRTypeInstTest{
            "SUBW_Overflow",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, 0 },
            { 1, std::numeric_limits<NativeWord>::max() },
            { 2, -1 },
        },
        
        /* Test SUBW underflow. */
        HartRTypeInstTest{
            "SUBW_Underflow",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, std::numeric_limits<NativeWord>::max() },
            { 1, 0 },
            { 2, 1 },
        },

        /* Test SUBW with rs1 greater than 2^31-1, upper bits should be ignored. */
        HartRTypeInstTest{
            "SUBW_IgnoreUpper",
            cpu::Opcode::OP_32,
            cpu::Funct3::ADDW,
            cpu::Funct7::SUBW,
            { 15, 5 },
            { 1, (1ull << 48) },
            { 2, -5 }
        },
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
