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
        /* Test ADDI with a immediate of zero. */
        HartITypeInstTest{
            "ADDI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ADDI,
            { 1, 0x100 },
            { 15, 0x100 },
            0
        },

        /* Test ADDI with a positive immediate. */
        HartITypeInstTest{
            "ADDI_PosImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ADDI,
            { 1, 120 },
            { 15, 50 },
            70
        },

        /* Test ADDI with a negative immediate. */
        HartITypeInstTest{
            "ADDI_NegImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ADDI,
            { 1, 30 },
            { 15, 50 },
            static_cast<Word>(-20)
        },

        /* Test ADDI with negative rs1 and immediate. */
        HartITypeInstTest{
            "ADDI_NegRs1NegImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ADDI,
            { 1, -50 },
            { 15, -30 },
            static_cast<Word>(-20)
        },

        /* Test ADDI with overflow. */
        HartITypeInstTest{
            "ADDI_Overflow",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ADDI,
            { 1, 0 },
            { 15, std::numeric_limits<NativeWord>::max() },
            1
        },

        /* Test ADDI with underflow. */
        HartITypeInstTest{
            "ADDI_Underflow",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ADDI,
            { 1, std::numeric_limits<NativeWord>::max() },
            { 15, 0 },
            static_cast<Word>(-1)
        },

        /* Test SLLI with a immediate of zero. */
        HartITypeInstTest{
            "SLLI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLLI,
            { 1, 50000 },
            { 15, 50000 },
            0
        },

        /* Test SLLI with a positive rs1. */
        HartITypeInstTest{
            "SLLI_PosRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLLI,
            { 1, 50000 << 8 },
            { 15, 50000 },
            8
        },

        /* Test SLLI with a negative rs1. */
        HartITypeInstTest{
            "SLLI_NegRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLLI,
            { 1, -50000 << 8 },
            { 15, -50000 },
            8
        },

        /* Test SLTI with imm == rs1, both positive. */
        HartITypeInstTest{
            "SLTI_PosEq",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 0 },
            { 15, 20 },
            20
        },

        /* Test SLTI with imm == rs1, both negative. */
        HartITypeInstTest{
            "SLTI_NegEq",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 0 },
            { 15, -20 },
            static_cast<Word>(-20)
        },

        /* Test SLTI with imm > rs1, both positive. */
        HartITypeInstTest{
            "SLTI_PosGreaterImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 1 },
            { 15, 20 },
            40
        },

        /* Test SLTI with imm > rs1, both negative. */
        HartITypeInstTest{
            "SLTI_NegGreaterImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 1 },
            { 15, -40 },
            static_cast<Word>(-20)
        },

        /* Test SLTI with imm > rs1, positive imm & negative rs1. */
        HartITypeInstTest{
            "SLTI_NegRs1PosImmGreaterImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 1 },
            { 15, -20 },
            20
        },

        /* Test SLTI with imm < rs1, both positive. */
        HartITypeInstTest{
            "SLTI_PosGreaterRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 0 },
            { 15, 40 },
            20
        },

        /* Test SLTI with imm < rs1, both negative. */
        HartITypeInstTest{
            "SLTI_NegGreaterRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 0 },
            { 15, -20 },
            static_cast<Word>(-40)
        },

        /* Test SLTI with imm < rs1, negative imm & positive rs1. */
        HartITypeInstTest{
            "SLTI_PosRs1NegImmGreaterRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTI,
            { 1, 0 },
            { 15, 20 },
            static_cast<Word>(-20)
        },

        /* Test SLTIU with imm == rs1, both positive. */
        HartITypeInstTest{
            "SLTIU_PosEq",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 0 },
            { 15, 20 },
            20
        },

        /* Test SLTIU with imm == rs1, both "negative". */
        HartITypeInstTest{
            "SLTIU_NegEq",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 0 },
            { 15, -20 },
            static_cast<Word>(-20)
        },

        /* Test SLTIU with imm > rs1, both positive. */
        HartITypeInstTest{
            "SLTIU_PosGreaterImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 1 },
            { 15, 20 },
            40
        },

        /* Test SLTIU with imm > rs1, both "negative". */
        HartITypeInstTest{
            "SLTIU_NegGreaterImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 1 },
            { 15, -40 },
            static_cast<Word>(-20)
        },

        /* Test SLTIU with imm > rs1, positive imm & "negative" rs1. */
        HartITypeInstTest{
            "SLTIU_NegRs1PosImmGreaterImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 0 },
            { 15, -20 },
            20
        },

        /* Test SLTIU with imm < rs1, both positive. */
        HartITypeInstTest{
            "SLTIU_PosGreaterRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 0 },
            { 15, 40 },
            20
        },

        /* Test SLTIU with imm < rs1, both negative. */
        HartITypeInstTest{
            "SLTIU_NegGreaterRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 0 },
            { 15, -20 },
            static_cast<Word>(-40)
        },

        /* Test SLTIU with imm < rs1, negative imm & positive rs1. */
        HartITypeInstTest{
            "SLTIU_PosRs1NegImmGreaterRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLTIU,
            { 1, 1 },
            { 15, 20 },
            static_cast<Word>(-20)
        },

        /* Test XORI with a immediate of zero. */
        HartITypeInstTest{
            "XORI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::XORI,
            { 1, 0xAABBCCDD },
            { 15, 0xAABBCCDD },
            0
        },

        /* Test XORI with a positive immediate. */
        HartITypeInstTest{
            "XORI_PosImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::XORI,
            { 1, 0xAA ^ 0x77 },
            { 15, 0xAA },
            0x77,
        },

        /* Test XORI with a negative immediate. */
        HartITypeInstTest{
            "XORI_NegImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::XORI,
            { 1, 0xAABB ^ -5 },
            { 15, 0xAABB },
            static_cast<Word>(-5)
        },

        /* Test SRLI with a immediate of zero. */
        HartITypeInstTest{
            "SRLI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, 50000 },
            { 15, 50000 },
            0
        },

        /* Test SRLI with a positive rs1. */
        HartITypeInstTest{
            "SRLI_PosRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, 50000 >> 8 },
            { 15, 50000 },
            8
        },

        /* Test SRLI with a negative rs1. */
        HartITypeInstTest{
            "SRLI_NegRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, static_cast<NativeWord>(-50000) >> 8 },
            { 15, -50000 },
            8
        },

        /* Test SRAI with a immediate of zero. */
        HartITypeInstTest{
            "SRAI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, -50000 },
            { 15, -50000 },
            (1 << 10u)
        },

        /* Test SRAI with a positive rs1. */
        HartITypeInstTest{
            "SRAI_PosRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, 50000 >> 8 },
            { 15, 50000 },
            8 | (1 << 10u)
        },

        /* Test SRAI with a negative rs1. */
        HartITypeInstTest{
            "SRAI_NegRs1",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, -50000 >> 8 },
            { 15, -50000 },
            8 | (1 << 10u)
        },

        /* Test ORI with a immediate of zero. */
        HartITypeInstTest{
            "ORI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ORI,
            { 1, 50000 },
            { 15, 50000 },
            0
        },

        /* Test ORI with a positive immediate. */
        HartITypeInstTest{
            "ORI_PosImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ORI,
            { 1, 50000 | 0x99 },
            { 15, 50000 },
            0x99
        },

        /* Test ORI with a negative immediate. */
        HartITypeInstTest{
            "ORI_NegImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ORI,
            { 1, 50000 | -20 },
            { 15, 50000 },
            static_cast<Word>(-20)
        },

        /* Test ANDI with a immediate of zero. */
        HartITypeInstTest{
            "ANDI_ZeroImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ANDI,
            { 1, 50000 & 0 },
            { 15, 50000 },
            0
        },

        /* Test ANDI with a positive immediate. */
        HartITypeInstTest{
            "ANDI_PosImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ANDI,
            { 1, 50000 & 0x99 },
            { 15, 50000 },
            0x99
        },

        /* Test ANDI with a negative immediate. */
        HartITypeInstTest{
            "ANDI_NegImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::ANDI,
            { 1, 50000 & -20 },
            { 15, 50000 },
            static_cast<Word>(-20)
        },
    }
};

} // namespace

TestResults Main([[maybe_unused]] Args args) {
    static HartTestSystem sys;

    sys.Initialize();

    return g_TestRunner.RunAll(&sys);
}

} // namespace test
} // namespace riscv