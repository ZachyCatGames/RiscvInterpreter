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
        /* Test SLLI shift with upper bit set. */
        HartITypeInstTest{
            "SLLI_UpperImm",
            cpu::Opcode::OP_IMM,
            cpu::Function::SLLI,
            { 1, 1ull << 32 },
            { 15, 1 },
            32
        },

        /* Test SRLI shift with upper bit set. */
        HartITypeInstTest{
            "SRLI_UpperImm",
            cpu::Opcode::OP_IMM,
            cpu::Function::SRLI,
            { 1, (1ull << 48) >> 32 },
            { 15, (1ull << 48) },
            32
        },

        /* Test SRAI shift with upper bit set. */
        HartITypeInstTest{
            "SRAI_UpperImm",
            cpu::Opcode::OP_IMM,
            cpu::Function::SRLI,
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
