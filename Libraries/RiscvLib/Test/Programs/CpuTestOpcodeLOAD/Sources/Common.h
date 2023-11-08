#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmuTest/test_TestFramework.h>
#include <RiscvEmuTest/cpu/test_HartTestCase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>

namespace riscv {
namespace test {

class TestInstLOAD : public HartSingleInstTestBase<TestInstLOAD> {
public:
    using AddrPairT = std::pair<Address, NativeWord>;
    constexpr TestInstLOAD(std::string_view name, cpu::Opcode op, cpu::Function func, RegPairT rd, RegPairT rs1, Word imm, AddrPairT memVal) :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, func, GetPairId(rd), GetPairId(rs1), imm)), name),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1),
        m_MemVal(memVal) {}
private:
    friend class HartSingleInstTestBase<TestInstLOAD>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Write our memVal to memory. */
        Result res = pSys->MemWriteDWord(static_cast<DWord>(std::get<1>(m_MemVal)), std::get<0>(m_MemVal));
        if(res.IsFailure()) {
            return res;
        }

        /* Write rs1. */
        pSys->WriteGPR(m_InitialRs1);

        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check rd value. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
    RegPairT m_InitialRs1;
    AddrPairT m_MemVal;
}; // class TestInstLOAD

} // namespace test
} // namespace riscv
