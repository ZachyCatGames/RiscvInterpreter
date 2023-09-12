#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_TestFramework.h>

/* TODO: Test misalignment. */

namespace riscv {
namespace test {

namespace {

class BranchInstTest : public HartSingleInstTestBase<BranchInstTest> {
public:
    constexpr BranchInstTest(std::string_view name, cpu::Opcode op, cpu::Funct3 f3, RegPairT rs1, RegPairT rs2, NativeWord initialPC, Word imm, bool branchExpected) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeBTypeInstruction(op, f3, GetPairId(rs1), GetPairId(rs2), imm)), name),
        m_InitialRs1(rs1),
        m_InitialRs2(rs2),
        m_InitialPC(initialPC),
        m_BranchOffset(imm),
        m_BranchExpected(branchExpected) {}
private:
    friend class HartSingleInstTestBase<BranchInstTest>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Initialize PC. */
        pSys->WritePC(m_InitialPC);

        /* Write rs1 & rs2. */
        pSys->WriteGPR(m_InitialRs1);
        pSys->WriteGPR(m_InitialRs2);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check that PC is what we expect it to be. */
        if(m_BranchExpected) {
            if(pSys->ReadPC() != m_InitialPC + m_BranchOffset) {
                return ResultRegValMismatch();
            }
        }
        else {
            if(pSys->ReadPC() != m_InitialPC) {
                return ResultRegValMismatch();
            }
        }

        return ResultSuccess();
    }
private:
    RegPairT m_InitialRs1;
    RegPairT m_InitialRs2;
    NativeWord m_InitialPC;
    Word m_BranchOffset;
    bool m_BranchExpected;
}; // class BranchInstTest

constexpr TestFramework g_TestRunner {
    &HartTestSystem::DefaultReset,


    std::tuple {
        /* Test BEQ with non-equal values. */
        BranchInstTest{
            "BEQ_NonEq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BEQ,
            { 1, 5 },
            { 2, 10 },
            0,
            0x10,
            false
        },

        /* Test BEQ with equal values. */
        BranchInstTest{
            "BEQ_Eq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BEQ,
            { 1, 5 },
            { 2, 5 },
            0,
            0x100,
            true
        },

        /* Test BNE with non-equal values. */
        BranchInstTest{
            "BNE_NonEq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BNE,
            { 1, 5 },
            { 2, 10 },
            0,
            0x10,
            true
        },

        /* Test BNE with equal values. */
        BranchInstTest{
            "BNE_Eq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BNE,
            { 1, 5 },
            { 2, 5 },
            0,
            0x100,
            false
        },

        /* Test BLT with equal rs1 and rs2. */
        BranchInstTest{
            "BLT_Eq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, 5 },
            { 2, 5 },
            0,
            0x100,
            false
        },

        /* Test BLT with rs1 greater than rs2, both positive. */
        BranchInstTest{
            "BLT_PosRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, 10 },
            { 2, 5 },
            0,
            0x100,
            false
        },

        /* Test BLT with rs1 less than rs2, both positive. */
        BranchInstTest{
            "BLT_PosRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, 5 },
            { 2, 10 },
            0,
            0x100,
            true
        },

        /* Test BLT with rs1 greater than rs2, both negative. */
        BranchInstTest{
            "BLT_NegRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, -5 },
            { 2, -10 },
            0,
            0x100,
            false
        },

        /* Test BLT with rs1 less than rs2, both negative. */
        BranchInstTest{
            "BLT_NegRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, -10 },
            { 2, -5 },
            0,
            0x100,
            true
        },

        /* Test BLT with positive rs1 and negative rs2. */
        BranchInstTest{
            "BLT_PosRs1NegRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, 10 },
            { 2, -10 },
            0,
            0x100,
            false
        },

        /* Test BLT with negative rs1 and positive rs2. */
        BranchInstTest{
            "BLT_NegRs1PosRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLT,
            { 1, -10 },
            { 2, 10 },
            0,
            0x100,
            true
        },

        /* Test BGE with equal rs1 and rs2. */
        BranchInstTest{
            "BGE_Eq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, 5 },
            { 2, 5 },
            0,
            0x100,
            true
        },

        /* Test BGE with rs1 greater than rs2, both positive. */
        BranchInstTest{
            "BGE_PosRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, 10 },
            { 2, 5 },
            0,
            0x100,
            true
        },

        /* Test BGE with rs1 less than rs2, both positive. */
        BranchInstTest{
            "BGE_PosRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, 5 },
            { 2, 10 },
            0,
            0x100,
            false
        },

        /* Test BGE with rs1 greater than rs2, both negative. */
        BranchInstTest{
            "BGE_NegRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, -5 },
            { 2, -10 },
            0,
            0x100,
            true
        },

        /* Test BGE with rs1 less than rs2, both negative. */
        BranchInstTest{
            "BGE_NegRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, -10 },
            { 2, -5 },
            0,
            0x100,
            false
        },

        /* Test BGE with positive rs1 and negative rs2. */
        BranchInstTest{
            "BGE_PosRs1NegRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, 10 },
            { 2, -10 },
            0,
            0x100,
            true
        },

        /* Test BGE with negative rs1 and positive rs2. */
        BranchInstTest{
            "BGE_NegRs1PosRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGE,
            { 1, -10 },
            { 2, 10 },
            0,
            0x100,
            false
        },





        /* Test BLTU with equal rs1 and rs2. */
        BranchInstTest{
            "BLTU_Eq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, 5 },
            { 2, 5 },
            0,
            0x100,
            false
        },

        /* Test BLTU with rs1 greater than rs2, both positive. */
        BranchInstTest{
            "BLTU_PosRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, 10 },
            { 2, 5 },
            0,
            0x100,
            false
        },

        /* Test BLTU with rs1 less than rs2, both positive. */
        BranchInstTest{
            "BLTU_PosRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, 5 },
            { 2, 10 },
            0,
            0x100,
            true
        },

        /* Test BLTU with rs1 greater than rs2, both negative. */
        BranchInstTest{
            "BLTU_NegRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, -5 },
            { 2, -10 },
            0,
            0x100,
            false
        },

        /* Test BLTU with rs1 less than rs2, both negative. */
        BranchInstTest{
            "BLTU_NegRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, -10 },
            { 2, -5 },
            0,
            0x100,
            true
        },

        /* Test BLTU with positive rs1 and negative rs2. */
        BranchInstTest{
            "BLTU_PosRs1NegRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, 10 },
            { 2, -10 },
            0,
            0x100,
            true
        },

        /* Test BLTU with negative rs1 and positive rs2. */
        BranchInstTest{
            "BLTU_NegRs1PosRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BLTU,
            { 1, -10 },
            { 2, 10 },
            0,
            0x100,
            false
        },

        /* Test BGEU with equal rs1 and rs2. */
        BranchInstTest{
            "BGEU_Eq",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, 5 },
            { 2, 5 },
            0,
            0x100,
            true
        },

        /* Test BGEU with rs1 greater than rs2, both positive. */
        BranchInstTest{
            "BGEU_PosRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, 10 },
            { 2, 5 },
            0,
            0x100,
            true
        },

        /* Test BGEU with rs1 less than rs2, both positive. */
        BranchInstTest{
            "BGEU_PosRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, 5 },
            { 2, 10 },
            0,
            0x100,
            false
        },

        /* Test BGEU with rs1 greater than rs2, both negative. */
        BranchInstTest{
            "BGEU_NegRs1Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, -5 },
            { 2, -10 },
            0,
            0x100,
            true
        },

        /* Test BGEU with rs1 less than rs2, both negative. */
        BranchInstTest{
            "BGEU_NegRs2Greater",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, -10 },
            { 2, -5 },
            0,
            0x100,
            false
        },

        /* Test BGEU with positive rs1 and negative rs2. */
        BranchInstTest{
            "BGEU_PosRs1NegRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, 10 },
            { 2, -10 },
            0,
            0x100,
            false
        },

        /* Test BGEU with negative rs1 and positive rs2. */
        BranchInstTest{
            "BGEU_NegRs1PosRs2",
            cpu::Opcode::BRANCH,
            cpu::Funct3::BGEU,
            { 1, -10 },
            { 2, 10 },
            0,
            0x100,
            true
        },
    }
};

} // namespace

extern constexpr std::string_view ProgramName = "CpuTestOpcodeBRANCH";

Result Main([[maybe_unused]] Args arg) {
    HartTestSystem sys;

    sys.Initialize();

    return g_TestRunner.RunAll(&sys);
}

} // namespace test
} // namespace riscv