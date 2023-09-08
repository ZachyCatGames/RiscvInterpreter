#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_EncodeInstruction.h>
#include <RiscvEmu/cpu/decoder/cpu_Opcodes.h>
#include <RiscvEmuTest/test_HartTestSystem.h>
#include <RiscvEmuTest/test_Result.ext.h>
#include <RiscvEmuTest/test_TestCaseBase.h>
#include <utility>

namespace riscv {
namespace test {

class HartCustomTest : public TestCaseBase<HartCustomTest, HartTestSystem> {
public:
    using FuncType = Result(*)(HartTestSystem* pSys);
    constexpr HartCustomTest(FuncType test) noexcept :
        TestCaseBase<HartCustomTest, HartTestSystem>("test"),
        m_Test(test) {}
private:
    friend class TestCaseBase<HartCustomTest, HartTestSystem>;
    Result TestImpl(HartTestSystem* pSys) const {
        return m_Test(pSys);
    }
private:
    FuncType m_Test;
}; // class HartCustomTest

template<typename Derived>
class HartSingleInstTestBase : public TestCaseBase<HartSingleInstTestBase<Derived>, HartTestSystem> {
private:
    friend class TestCaseBase<HartSingleInstTestBase<Derived>, HartTestSystem>;
    Result RunImpl(HartTestSystem* pSys) const {
        /* Perform specific initialization. */
        Result res = static_cast<const Derived*>(this)->Initialize(pSys);
        if(res.IsFailure()) {
            return res;
        }

        /* Run the instruction. */
        res = pSys->ExecuteInstruction(m_Inst);
        if(res.GetValue() != m_ExpectedResult.GetValue()) {
            return res;
        }

        /* Perform check. */
        return static_cast<const Derived*>(this)->Check(pSys);
    }
protected:
    constexpr HartSingleInstTestBase(cpu::Instruction inst, std::string_view name, Result expectedRes) noexcept :
        TestCaseBase<HartSingleInstTestBase<Derived>, HartTestSystem>(name),
        m_Inst(inst),
        m_ExpectedResult(expectedRes) {}

    using RegPairT = std::pair<int, NativeWord>;

    static constexpr auto GetPairId(RegPairT pair) noexcept { return std::get<0>(pair); }
    static constexpr auto GetPairVal(RegPairT pair) noexcept { return std::get<1>(pair); }    
private:
    cpu::Instruction m_Inst;
    Result m_ExpectedResult;
}; // class HartSingleInstTestBase

class HartSimpleRTypeTest : public HartSingleInstTestBase<HartSimpleRTypeTest> {
public:
    constexpr HartSimpleRTypeTest(std::string_view name, Result expectedRes, cpu::Opcode op, cpu::Funct3 f3, cpu::Funct7 f7, RegPairT rd, RegPairT rs1, RegPairT rs2) noexcept :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeRTypeInstruction(op, f3, f7, GetPairId(rd), GetPairId(rs1), GetPairId(rs2))), name, expectedRes),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1),
        m_InitialRs2(rs2) {}
private:
    friend class HartSingleInstTestBase<HartSimpleRTypeTest>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Set register values. */
        pSys->WriteGPR(m_InitialRs1);
        pSys->WriteGPR(m_InitialRs2);
        return ResultSuccess();
    }

    Result Check(HartTestSystem* pSys) const {
        /* Check rd value. */
        return pSys->CheckGPR(m_ExpectedRd);
    }
private:
    RegPairT m_ExpectedRd;
    RegPairT m_InitialRs1;
    RegPairT m_InitialRs2;
}; // class HartSimpleRTypeTest

class HartSimpleITypeTest : public HartSingleInstTestBase<HartSimpleITypeTest> {
public:
    constexpr HartSimpleITypeTest(std::string_view name, Result expectedRes, cpu::Opcode op, cpu::Funct3 f3, RegPairT rd, RegPairT rs1, Word imm) noexcept :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, f3, GetPairId(rd), GetPairId(rs1), imm)), name, expectedRes),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1) {}
private:
    friend class HartSingleInstTestBase<HartSimpleITypeTest>;
    Result Initialize(HartTestSystem* pSys) const {
        /* Set rs1 value. */
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
}; // class HartSimpleITypeTest

} // namespace test
} // namespace riscv