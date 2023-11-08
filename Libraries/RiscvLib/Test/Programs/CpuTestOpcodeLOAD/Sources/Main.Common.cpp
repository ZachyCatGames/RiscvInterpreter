#include "Common.h"

/* TODO: Load Access/Page faults. */

namespace riscv {
namespace test {

namespace {

constexpr TestFramework g_TestRunner(
    &HartTestSystem::DefaultReset,

    std::tuple {
        /* Test LB with an offset of 0. */
        TestInstLOAD{
            "LB_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LB,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LB with a positive offset. */
        TestInstLOAD{
            "LB_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LB,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11 }
        },

        /* Test LB with a negative offset. */
        TestInstLOAD{
            "LB_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LB,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LB with a signed value. */
        TestInstLOAD{
            "LB_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Function::LB,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LH with an offset of 0. */
        TestInstLOAD{
            "LH_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LH,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LH with a positive offset. */
        TestInstLOAD{
            "LH_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LH,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122 }
        },

        /* Test LH with a negative offset. */
        TestInstLOAD{
            "LH_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LH,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LH with a signed value. */
        TestInstLOAD{
            "LH_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Function::LH,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LW with an offset of 0. */
        TestInstLOAD{
            "LW_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LW,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LW with a positive offset. */
        TestInstLOAD{
            "LW_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LW,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11223344 }
        },

        /* Test LW with a negative offset. */
        TestInstLOAD{
            "LW_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LW,
            { 1, 0x11223344 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11223344 }
        },

        /* Test LW with a signed value. */
        TestInstLOAD{
            "LW_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Function::LW,
            { 1, -69 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, -69 }
        },

        /* Test LBU with an offset of 0. */
        TestInstLOAD{
            "LBU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LBU,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LBU with a positive offset. */
        TestInstLOAD{
            "LBU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LBU,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x11 }
        },

        /* Test LBU with a negative offset. */
        TestInstLOAD{
            "LBU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LBU,
            { 1, 0x11 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x11 }
        },

        /* Test LBU with a signed value. */
        TestInstLOAD{
            "LBU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Function::LBU,
            { 1, 0x80 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x80 }
        },

        /* Test LHU with an offset of 0. */
        TestInstLOAD{
            "LHU_ZeroImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LHU,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LHU with a positive offset. */
        TestInstLOAD{
            "LHU_PosImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LHU,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0x1122 }
        },

        /* Test LHU with a negative offset. */
        TestInstLOAD{
            "LHU_NegImmPosMem",
            cpu::Opcode::LOAD,
            cpu::Function::LHU,
            { 1, 0x1122 },
            { 15, HartTestSystem::MemoryAddress + 0x100 },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0x1122 }
        },

        /* Test LHU with a signed value. */
        TestInstLOAD{
            "LHU_ZeroImmNegMem",
            cpu::Opcode::LOAD,
            cpu::Function::LHU,
            { 1, 0x8000 },
            { 15, HartTestSystem::MemoryAddress },
            0,
            { HartTestSystem::MemoryAddress, 0x8000 }
        },
    }
);

} // namespace

TestResults Main([[maybe_unused]] Args args) {
    static HartTestSystem sys;

    sys.Initialize();

    return g_TestRunner.RunAll(&sys);
}

} // namespace test
} // namespace riscv

