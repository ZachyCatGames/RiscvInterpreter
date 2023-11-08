#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmuTest/cpu/test_HartTestCase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>

namespace riscv {
namespace test {

class TestInstSTORE : public HartSingleInstTestBase<TestInstSTORE> {
public:
    using AddrPairT = std::pair<Address, NativeWord>;
    constexpr TestInstSTORE(std::string_view name, cpu::Opcode op, cpu::Function func, RegPairT rs1, RegPairT rs2, Word imm, AddrPairT memExpect) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeSTypeInstruction(op, func, GetPairId(rs1), GetPairId(rs2), imm)), name),
        m_ExpectedMemVal(memExpect),
        m_InitialRs1(rs1),
        m_InitialRs2(rs2) {}
private:
    friend class HartSingleInstTestBase<TestInstSTORE>;
    Result Initialize(HartTestSystem* pSys) const{
        /* Write rs1 & rs2. */
        pSys->WriteGPR(m_InitialRs1);
        pSys->WriteGPR(m_InitialRs2);
    
        return ResultSuccess();
    }


    Result Check(HartTestSystem* pSys) const{
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

} // namespace test
} // namespace riscv
