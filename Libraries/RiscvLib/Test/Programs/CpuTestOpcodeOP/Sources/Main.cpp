#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <limits>

namespace riscv {
namespace test {

namespace {

constexpr TestFramework g_TestRunner {
    &HartTestSystem::DefaultReset,

    std::tuple {
        /* Test ADD with both rs1 and rs2 as zero. */
        HartSimpleRTypeTest{
            "ADD_ZeroRs1ZeroRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, 0 },
            { 1, 0 },
            { 2, 0 }
        },

        /* Test ADD with rs1 and rs2 as positive values. */
        HartSimpleRTypeTest{
            "ADD_PosRs1PosRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, 50 },
            { 1, 30 },
            { 2, 20 }
        },

        /* Test ADD with rs1 and rs2 as negative values. */
        HartSimpleRTypeTest{
            "ADD_NegRs1NegRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, -50 },
            { 1, -30 },
            { 2, -20 },
        },

        /* Test ADD with positive rs1 and negative rs2. */
        HartSimpleRTypeTest{
            "ADD_PosRs1NegRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, 0 },
            { 1, 30 },
            { 2, -30 },
        },

        /* Test ADD with negative rs1 and positive rs2. */
        HartSimpleRTypeTest{
            "ADD_NegRs1PosRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, -10 },
            { 1, -30 },
            { 2, 20 }
        },

        /* Test ADD overflow. */
        HartSimpleRTypeTest{
            "ADD_Overflow",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, 0 },
            { 1, std::numeric_limits<NativeWord>::max() },
            { 2, 1 },
        },

        /* Test ADD underflow. */
        HartSimpleRTypeTest{
            "ADD_Underflow",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::ADD,
            { 15, std::numeric_limits<NativeWord>::max() },
            { 1, 0 },
            { 2, -1 },
        },

        /* Test SUB with both rs1 and rs2 as zero. */
        HartSimpleRTypeTest{
            "SUB_ZeroRs1ZeroRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, 0 },
            { 1, 0 },
            { 2, 0 }
        },

        /* Test SUB with rs1 and rs2 as positive values. */
        HartSimpleRTypeTest{
            "SUB_PosRs1PosRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, 10 },
            { 1, 30 },
            { 2, 20 }
        },

        /* Test SUB with rs1 and rs2 as negative values. */
        HartSimpleRTypeTest{
            "SUB_NegRs1NegRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, -10 },
            { 1, -30 },
            { 2, -20 },
        },

        /* Test SUB with positive rs1 and negative rs2. */
        HartSimpleRTypeTest{
            "SUB_PosRs1NegRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, 60 },
            { 1, 30 },
            { 2, -30 },
        },

        /* Test SUB with negative rs1 and positive rs2. */
        HartSimpleRTypeTest{
            "SUB_NegRs1PosRs2",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, -50 },
            { 1, -30 },
            { 2, 20 }
        },

        /* Test SUB overflow. */
        HartSimpleRTypeTest{
            "SUB_Overflow",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, 0 },
            { 1, std::numeric_limits<NativeWord>::max() },
            { 2, -1 },
        },
        
        /* Test SUB underflow. */
        HartSimpleRTypeTest{
            "SUB_Underflow",
            cpu::Opcode::OP,
            cpu::Funct3::ADD,
            cpu::Funct7::SUB,
            { 15, std::numeric_limits<NativeWord>::max() },
            { 1, 0 },
            { 2, 1 },
        },

        /* Test SLL with a rs2ediate of zero. */
        HartSimpleRTypeTest{
            "SLL_ZeroRs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLL,
            cpu::Funct7::Zero,
            { 15, 50000 },
            { 1, 50000 },
            { 2, 0 }
        },

        /* Test SLL with a positive rs1. */
        HartSimpleRTypeTest{
            "SLL_PosRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLL,
            cpu::Funct7::Zero,
            { 15, 50000 << 8 },
            { 1, 50000 },
            { 2, 8 }
        },

        /* Test SLL with a negative rs1. */
        HartSimpleRTypeTest{
            "SLL_NegRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLL,
            cpu::Funct7::Zero,
            { 15, static_cast<NativeWord>(-50000) << 8 },
            { 1, -50000 },
            { 2, 8 }
        },

        /* Test SLT with rs2 == rs1, both positive. */
        HartSimpleRTypeTest{
            "SLT_PosEq",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, 20 },
            { 2, 20 },
        },

        /* Test SLT with rs2 == rs1, both negative. */
        HartSimpleRTypeTest{
            "SLT_NegEq",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, -20 },
            { 2, -20 }
        },

        /* Test SLT with rs2 > rs1, both positive. */
        HartSimpleRTypeTest{
            "SLT_PosGreaterRs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 1 },
            { 1, 20 },
            { 2, 40 }
        },

        /* Test SLT with rs2 > rs1, both negative. */
        HartSimpleRTypeTest{
            "SLT_NegGreaterRs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 1 },
            { 1, -40 },
            { 2, -20 }
        },

        /* Test SLT with rs2 > rs1, positive rs2 & negative rs1. */
        HartSimpleRTypeTest{
            "SLT_NegRs1PosRs2Greaterrs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 1 },
            { 1, static_cast<NativeWord>(-20) },
            { 2, 20 }
        },

        /* Test SLT with rs2 < rs1, both positive. */
        HartSimpleRTypeTest{
            "SLT_PosGreaterRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, 40 },
            { 2, 20 }
        },

        /* Test SLT with rs2 < rs1, both negative. */
        HartSimpleRTypeTest{
            "SLT_NegGreaterRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, -20 },
            { 2, -40 },
        },

        /* Test SLT with rs2 < rs1, negative rs2 & positive rs1. */
        HartSimpleRTypeTest{
            "SLT_PosRs1NegRs2GreaterRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLT,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, 20 },
            { 2, -20 }
        },

        /* Test SLTU with rs2 == rs1, both positive. */
        HartSimpleRTypeTest{
            "SLTU_PosEq",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, 20 },
            { 2, 20 }
        },

        /* Test SLTU with rs2 == rs1, both "negative". */
        HartSimpleRTypeTest{
            "SLTU_NegEq",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, -20 },
            { 2, -20 }
        },

        /* Test SLTU with rs2 > rs1, both positive. */
        HartSimpleRTypeTest{
            "SLTU_PosGreaterrs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 1 },
            { 1, 20 },
            { 2, 40 }
        },

        /* Test SLTU with rs2 > rs1, both "negative". */
        HartSimpleRTypeTest{
            "SLTU_NegGreaterrs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 1 },
            { 1, -40 },
            { 2, -20 }
        },

        /* Test SLTU with rs2 > rs1, positive rs2 & "negative" rs1. */
        HartSimpleRTypeTest{
            "SLTU_NegRs1PosRs2Greaterrs2",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, -20 },
            { 2, 20 }
        },

        /* Test SLTU with rs2 < rs1, both positive. */
        HartSimpleRTypeTest{
            "SLTU_PosGreaterRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, 40 },
            { 2, 20 }
        },

        /* Test SLTU with rs2 < rs1, both negative. */
        HartSimpleRTypeTest{
            "SLTU_NegGreaterRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 0 },
            { 1, -20 },
            { 2, -40 }
        },

        /* Test SLTU with rs2 < rs1, negative rs2 & positive rs1. */
        HartSimpleRTypeTest{
            "SLTU_PosRs1NegRs2GreaterRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SLTU,
            cpu::Funct7::Zero,
            { 15, 1 },
            { 1, 20 },
            { 2, -20 }
        },

        /* Test XOR with a rs2 of zero. */
        HartSimpleRTypeTest{
            "XOR_ZeroImm",
            cpu::Opcode::OP,
            cpu::Funct3::XOR,
            cpu::Funct7::Zero,
            { 15, 0xAABBCCDD },
            { 1, 0xAABBCCDD },
            { 2, 0 }
        },

        /* Test XOR with a positive rs2. */
        HartSimpleRTypeTest{
            "XOR_PosImm",
            cpu::Opcode::OP,
            cpu::Funct3::XOR,
            cpu::Funct7::Zero,
            { 15, 0xAA ^ 0x77 },
            { 1, 0xAA },
            { 2, 0x77 }
        },

        /* Test XOR with a negative rs2. */
        HartSimpleRTypeTest{
            "XOR_NegImm",
            cpu::Opcode::OP,
            cpu::Funct3::XOR,
            cpu::Funct7::Zero,
            { 15, 0xAABB ^ -5 },
            { 1, 0xAABB },
            { 2, -5 }
        },

        /* Test SRL with a rs2 of zero. */
        HartSimpleRTypeTest{
            "SRL_ZeroImm",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRL,
            { 15, 50000 },
            { 1, 50000 },
            { 2, 0 }
        },

        /* Test SRL with a positive rs1. */
        HartSimpleRTypeTest{
            "SRL_PosRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRL,
            { 15, 50000 >> 8 },
            { 1, 50000 },
            { 2, 8 }
        },

        /* Test SRL with a negative rs1. */
        HartSimpleRTypeTest{
            "SRL_NegRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRL,
            { 15, static_cast<NativeWord>(-50000) >> 8 },
            { 1, -50000 },
            { 2, 8 }
        },

        /* Test that upper bits get chopped off in SRL. */
        HartSimpleRTypeTest{
            "SRL_Truncate",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRL,
            { 15, 50000 >> 8 },
            { 1, 50000 },
            { 2, 8 | (1 << 8) }
        },

        /* Test SRA with a rs2 of zero. */
        HartSimpleRTypeTest{
            "SRA_ZeroImm",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRA,
            { 15, -50000 },
            { 1, -50000 },
            { 2, 0 }
        },

        /* Test SRA with a positive rs1. */
        HartSimpleRTypeTest{
            "SRA_PosRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRA,
            { 15, 50000 >> 8 },
            { 1, 50000 },
            { 2, 8 }
        },

        /* Test SRA with a negative rs1. */
        HartSimpleRTypeTest{
            "SRA_NegRs1",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRA,
            { 15, -50000 >> 8 },
            { 1, -50000 },
            { 2, 8 }
        },

        /* Test that upper bits get chopped off in SRA. */
        HartSimpleRTypeTest{
            "SRA_Truncate",
            cpu::Opcode::OP,
            cpu::Funct3::SRL,
            cpu::Funct7::SRA,
            { 15, 50000 >> 8 },
            { 1, 50000 },
            { 2, 8 | (1 << 8) }
        },

        /* Test OR with a immediate of zero. */
        HartSimpleRTypeTest{
            "OR_ZeroImm",
            cpu::Opcode::OP,
            cpu::Funct3::OR,
            cpu::Funct7::Zero,
            { 15, 50000 },
            { 1, 50000 },
            { 2, 0 }
        },

        /* Test OR with a positive immediate. */
        HartSimpleRTypeTest{
            "OR_PosImm",
            cpu::Opcode::OP,
            cpu::Funct3::OR,
            cpu::Funct7::Zero,
            { 15, 50000 | 0x99 },
            { 1, 50000 },
            { 2, 0x99 }
        },

        /* Test OR with a negative immediate. */
        HartSimpleRTypeTest{
            "OR_NegImm",
            cpu::Opcode::OP,
            cpu::Funct3::OR,
            cpu::Funct7::Zero,
            { 15, 50000 | -20 },
            { 1, 50000 },
            { 2, -20 }
        },

        /* Test AND with a immediate of zero. */
        HartSimpleRTypeTest{
            "AND_ZeroImm",
            cpu::Opcode::OP,
            cpu::Funct3::AND,
            cpu::Funct7::Zero,
            { 15, 50000 & 0 },
            { 1, 50000 },
            { 2, 0 }
        },

        /* Test AND with a positive immediate. */
        HartSimpleRTypeTest{
            "AND_PosImm",
            cpu::Opcode::OP,
            cpu::Funct3::AND,
            cpu::Funct7::Zero,
            { 15, 50000 & 0x99 },
            { 1, 50000 },
            { 2, 0x99 }
        },

        /* Test AND with a negative immediate. */
        HartSimpleRTypeTest{
            "AND_NegImm",
            cpu::Opcode::OP,
            cpu::Funct3::AND,
            cpu::Funct7::Zero,
            { 15, 50000 & -20 },
            { 1, 50000 },
            { 2, -20 }
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
