#include <RiscvEmuTest/cpu/test_HartTestCase.h>

namespace riscv {
namespace test {

Result HartRTypeInstTest::Initialize(HartTestSystem* pSys) const {
    /* Set register values. */
    pSys->WriteGPR(m_InitialRs1);
    pSys->WriteGPR(m_InitialRs2);
    return ResultSuccess();
}

Result HartRTypeInstTest::Check(HartTestSystem* pSys) const {
    /* Check rd value. */
    return pSys->CheckGPR(m_ExpectedRd);
}

Result HartITypeInstTest::Initialize(HartTestSystem* pSys) const {
    /* Set rs1 value. */
    pSys->WriteGPR(m_InitialRs1);
    return ResultSuccess();
}

Result HartITypeInstTest::Check(HartTestSystem* pSys) const {
    /* Check rd value. */
    return pSys->CheckGPR(m_ExpectedRd);
}

} // namespace test
} // namespace riscv
