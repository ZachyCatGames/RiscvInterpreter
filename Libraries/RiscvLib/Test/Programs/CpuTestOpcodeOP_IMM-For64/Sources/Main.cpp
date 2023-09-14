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

    std::tuple{
        /* Test SLLI shift with upper bit set. */
        HartSimpleITypeTest{
            "SLLI_UpperImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SLLI,
            { 1, 1ull << 32 },
            { 15, 1 },
            32
        },

        /* Test SRLI shift with upper bit set. */
        HartSimpleITypeTest{
            "SRLI_UpperImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, (1ull << 48) >> 32 },
            { 15, (1ull << 48) },
            32
        },

        /* Test SRAI shift with upper bit set. */
        HartSimpleITypeTest{
            "SRAI_UpperImm",
            cpu::Opcode::OP_IMM,
            cpu::Funct3::SRLI,
            { 1, (1ull << 48) >> 32 },
            { 15, (1ull << 48) },
            32 | (1 << 10)
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
