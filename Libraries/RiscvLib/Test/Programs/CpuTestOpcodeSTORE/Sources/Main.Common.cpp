#include "Common.h"

/* TODO: Store Access/Page faults. */

namespace riscv {
namespace test {

namespace {

Result CustomReset(HartTestSystem* pSys) {
    /* Call default reset. */
    Result res = HartTestSystem::DefaultReset(pSys);
    if(res.IsFailure()) {
        return res;
    }

    /* Clear memory. */
    return pSys->ClearMem();
}

constexpr TestFramework g_TestRunner{
    &CustomReset,

    std::tuple{
        /* Test SB with no offset. */
        TestInstSTORE{
            "SB_ZeroImm",
            cpu::Opcode::STORE,
            cpu::Function::SB,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0,
            { HartTestSystem::MemoryAddress, 0xDD }
        },

        /* Test SB with a positive offset. */
        TestInstSTORE{
            "SB_PosImm",
            cpu::Opcode::STORE,
            cpu::Function::SB,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xDD }
        },

        /* Test SB with a negative offset. */
        TestInstSTORE{
            "SB_NegImm",
            cpu::Opcode::STORE,
            cpu::Function::SB,
            { 1, HartTestSystem::MemoryAddress + 0x100 },
            { 15, 0xAABBCCDD },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0xDD }
        },

        /* Test SH with no offset. */
        TestInstSTORE{
            "SH_ZeroImm",
            cpu::Opcode::STORE,
            cpu::Function::SH,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0,
            { HartTestSystem::MemoryAddress, 0xCCDD }
        },

        /* Test SH with a positive offset. */
        TestInstSTORE{
            "SH_PosImm",
            cpu::Opcode::STORE,
            cpu::Function::SH,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xCCDD }
        },

        /* Test SH with a negative offset. */
        TestInstSTORE{
            "SH_NegImm",
            cpu::Opcode::STORE,
            cpu::Function::SH,
            { 1, HartTestSystem::MemoryAddress + 0x100 },
            { 15, 0xAABBCCDD },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0xCCDD }
        },

        /* Test SW with no offset. */
        TestInstSTORE{
            "SW_ZeroImm",
            cpu::Opcode::STORE,
            cpu::Function::SW,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0,
            { HartTestSystem::MemoryAddress, 0xAABBCCDD }
        },

        /* Test SW with a positive offset. */
        TestInstSTORE{
            "SW_PosImm",
            cpu::Opcode::STORE,
            cpu::Function::SW,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xAABBCCDD }
        },

        /* Test SW with a negative offset. */
        TestInstSTORE{
            "SW_NegImm",
            cpu::Opcode::STORE,
            cpu::Function::SW,
            { 1, HartTestSystem::MemoryAddress + 0x100 },
            { 15, 0xAABBCCDD },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0xAABBCCDD }
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
