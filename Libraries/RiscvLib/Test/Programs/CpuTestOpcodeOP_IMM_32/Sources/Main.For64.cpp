#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmuTest/cpu/test_HartTestCase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>
#include <limits>

namespace riscv {
namespace test {

namespace {

constexpr TestFramework g_TestRunner {
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test ADDIW with a immediate of zero. */
        HartITypeInstTest{
            "ADDIW_ZeroImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, 0x100 },
            { 15, 0x100 },
            0
        },

        /* Test ADDIW with a positive immediate. */
        HartITypeInstTest{
            "ADDIW_PosImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, 120 },
            { 15, 50 },
            70
        },

        /* Test ADDIW with a negative immediate. */
        HartITypeInstTest{
            "ADDIW_NegImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, 30 },
            { 15, 50 },
            static_cast<Word>(-20)
        },

        /* Test ADDIW with negative rs1 and immediate. */
        HartITypeInstTest{
            "ADDIW_NegRs1NegImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, -50 },
            { 15, -30 },
            static_cast<Word>(-20)
        },

        /* Test ADDIW with overflow. */
        HartITypeInstTest{
            "ADDIW_Overflow",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, 0 },
            { 15, std::numeric_limits<Word>::max() },
            1
        },

        /* Test ADDIW with underflow. */
        HartITypeInstTest{
            "ADDIW_Underflow",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, std::numeric_limits<NativeWord>::max() },
            { 15, 0 },
            static_cast<Word>(-1)
        },

        /* Test ADDIW with rs1 greater than 2^31-1, upper bits should be ignored. */
        HartITypeInstTest{
            "ADDIW_IgnoreUpper",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::ADDIW,
            { 1, 5 },
            { 15, (1ull << 48) },
            5
        },

        /* Test SLLIW with a immediate of zero. */
        HartITypeInstTest{
            "SLLIW_ZeroImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SLLIW,
            { 1, 50000 },
            { 15, 50000 },
            0
        },

        /* Test SLLIW with a positive rs1. */
        HartITypeInstTest{
            "SLLIW_PosRs1",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SLLIW,
            { 1, 50000 << 8 },
            { 15, 50000 },
            8
        },

        /* Test SLLIW with a negative rs1. */
        HartITypeInstTest{
            "SLLIW_NegRs1",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SLLIW,
            { 1, -50000 << 8 },
            { 15, -50000 },
            8
        },

        /* Test that SLLIW sign extends rd. */
        HartITypeInstTest{
            "SLLIW_SignExt",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SLLIW,
            { 1, std::numeric_limits<NativeWord>::max() << 8 },
            { 15, 0xFFFFFFFF },
            8
        },

        /* Test SRLIW with a immediate of zero. */
        HartITypeInstTest{
            "SRLIW_ZeroImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, 50000 },
            { 15, 50000 },
            0
        },

        /* Test SRLIW with a positive rs1. */
        HartITypeInstTest{
            "SRLIW_PosRs1",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, 50000 >> 8 },
            { 15, 50000 },
            8
        },

        /* Test SRLIW with a negative rs1. */
        HartITypeInstTest{
            "SRLIW_NegRs1",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, static_cast<Word>(-50000) >> 8 },
            { 15, -50000 },
            8
        },

        /* Test that SRLIW sign extends rd. */
        HartITypeInstTest{
            "SRLIW_SignExt",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, std::numeric_limits<NativeWord>::max() << 8 },
            { 15, 0xFFFFFF00 },
            0
        },

        /* Test SRAIW with a immediate of zero. */
        HartITypeInstTest{
            "SRAIW_ZeroImm",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, -50000 },
            { 15, -50000 },
            (1 << 10u)
        },

        /* Test SRAIW with a positive rs1. */
        HartITypeInstTest{
            "SRAIW_PosRs1",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, 50000 >> 8 },
            { 15, 50000 },
            8 | (1 << 10u)
        },

        /* Test SRAIW with a negative rs1. */
        HartITypeInstTest{
            "SRAIW_NegRs1",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, -50000 >> 8 },
            { 15, -50000 },
            8 | (1 << 10u)
        },

        /* Test that SRAIW sign extends rd. */
        HartITypeInstTest{
            "SRAIW_SignExt",
            cpu::Opcode::OP_IMM_32,
            cpu::Funct3::SRLIW,
            { 1, std::numeric_limits<NativeWord>::max() << 8 },
            { 15, 0xFFFFFF00 },
            0 | (1 << 10u)
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
