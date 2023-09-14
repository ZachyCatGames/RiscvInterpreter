#include "Common.h"

namespace riscv {
namespace test {

namespace {

constexpr TestFramework g_TestRunner {
    &HartTestSystem::DefaultReset,

    std::tuple{
        /* Test LD with an offset of 0. */
        TestInstLOAD{
            "LD_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LD,
            { 1, 0x1122334455667788ull },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122334455667788ull }
        },

        /* Test LD with a positive offset. */
        TestInstLOAD{
            "LD_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LD,
            { 1, 0x1122334455667788ull },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122334455667788ull }
        },

        /* Test LD with a negative offset. */
        TestInstLOAD{
            "LD_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LD,
            { 1, 0x1122334455667788ull },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122334455667788ull }
        },

        /* Test LD with a signed value. */
        TestInstLOAD{
            "LD_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LD,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LWU with an offset of 0. */
        TestInstLOAD{
            "LWU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LWU,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LWU with a positive offset. */
        TestInstLOAD{
            "LWU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LWU,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11223344 }
        },

        /* Test LWU with a negative offset. */
        TestInstLOAD{
            "LWU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LWU,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LWU with a signed value. */
        TestInstLOAD{
            "LWU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Funct3::LWU,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11223344 }
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

