#include "Common.h"

namespace riscv {
namespace test {

namespace {

constexpr TestFramework g_TestRunner{
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test SD with no offset. */
        TestInstSTORE{
            "SD_ZeroImm",
            cpu::Opcode::STORE,
            cpu::Function::SD,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDDEEFF1122 },
            0,
            { HartTestSystem::MemoryAddress, 0xAABBCCDDEEFF1122 }
        },

        /* Test SD with a positive offset. */
        TestInstSTORE{
            "SD_PosImm",
            cpu::Opcode::STORE,
            cpu::Function::SD,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDDEEFF1122 },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xAABBCCDDEEFF1122 }
        },

        /* Test SD with a negative offset. */
        TestInstSTORE{
            "SD_NegImm",
            cpu::Opcode::STORE,
            cpu::Function::SD,
            { 1, HartTestSystem::MemoryAddress + 0x100 },
            { 15, 0xAABBCCDDEEFF1122 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0xAABBCCDDEEFF1122 }
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

