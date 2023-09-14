#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>

/* TODO: Store Access/Page faults. */

namespace riscv {
namespace test {

namespace {

class TestInstSTORE : public HartSingleInstTestBase<TestInstSTORE> {
public:
    using AddrPairT = std::pair<Address, NativeWord>;
    constexpr TestInstSTORE(std::string_view name, cpu::Opcode op, cpu::Funct3 f3, RegPairT rs1, RegPairT rs2, Word imm, AddrPairT memExpect) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeSTypeInstruction(op, f3, GetPairId(rs1), GetPairId(rs2), imm)), name),
        m_ExpectedMemVal(memExpect),
        m_InitialRs1(rs1),
        m_InitialRs2(rs2) {}
private:
    friend class HartSingleInstTestBase<TestInstSTORE>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Write rs1 & rs2. */
        pSys->WriteGPR(m_InitialRs1);
        pSys->WriteGPR(m_InitialRs2);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Read memory. */
        DWord out = 0;
        Result res = pSys->MemReadDWord(&out, std::get<0>(m_ExpectedMemVal));
        if(res.IsFailure()) {
            return res;
        }

        /* Check that the data read is correct. */
        if(out != std::get<1>(m_ExpectedMemVal)) {
            return ResultMemValMismatch();
        }

        return ResultSuccess();
    }
private:
    AddrPairT m_ExpectedMemVal;
    RegPairT m_InitialRs1;
    RegPairT m_InitialRs2;
}; // class TestInstSTORE

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
            cpu::Funct3::SB,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0,
            { HartTestSystem::MemoryAddress, 0xDD }
        },

        /* Test SB with a positive offset. */
        TestInstSTORE{
            "SB_PosImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SB,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xDD }
        },

        /* Test SB with a negative offset. */
        TestInstSTORE{
            "SB_NegImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SB,
            { 1, HartTestSystem::MemoryAddress + 0x100 },
            { 15, 0xAABBCCDD },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0xDD }
        },

        /* Test SH with no offset. */
        TestInstSTORE{
            "SH_ZeroImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SH,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0,
            { HartTestSystem::MemoryAddress, 0xCCDD }
        },

        /* Test SH with a positive offset. */
        TestInstSTORE{
            "SH_PosImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SH,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xCCDD }
        },

        /* Test SH with a negative offset. */
        TestInstSTORE{
            "SH_NegImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SH,
            { 1, HartTestSystem::MemoryAddress + 0x100 },
            { 15, 0xAABBCCDD },
            static_cast<Word>(-0x100),
            { HartTestSystem::MemoryAddress, 0xCCDD }
        },

        /* Test SW with no offset. */
        TestInstSTORE{
            "SW_ZeroImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SW,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0,
            { HartTestSystem::MemoryAddress, 0xAABBCCDD }
        },

        /* Test SW with a positive offset. */
        TestInstSTORE{
            "SW_PosImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SW,
            { 1, HartTestSystem::MemoryAddress },
            { 15, 0xAABBCCDD },
            0x100,
            { HartTestSystem::MemoryAddress + 0x100, 0xAABBCCDD }
        },

        /* Test SW with a negative offset. */
        TestInstSTORE{
            "SW_NegImm",
            cpu::Opcode::STORE,
            cpu::Funct3::SW,
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
