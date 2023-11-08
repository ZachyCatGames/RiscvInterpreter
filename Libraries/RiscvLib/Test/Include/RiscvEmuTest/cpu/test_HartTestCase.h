#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_EncodeInstruction.h>
#include <RiscvEmu/cpu/cpu_Opcodes.h>
#include <RiscvEmuTest/cpu/test_HartTestCaseBase.h>

namespace riscv {
namespace test {

class HartRTypeInstTest : public HartSingleInstTestBase<HartRTypeInstTest> {
public:
    constexpr HartRTypeInstTest(std::string_view name, cpu::Opcode op, cpu::Function func, RegPairT rd, RegPairT rs1, RegPairT rs2) noexcept :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeRTypeInstruction(op, func, GetPairId(rd), GetPairId(rs1), GetPairId(rs2))), name),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1),
        m_InitialRs2(rs2) {}
private:
    friend class HartSingleInstTestBase<HartRTypeInstTest>;
    Result Initialize(HartTestSystem* pSys) const;
    Result Check(HartTestSystem* pSys) const;
private:
    RegPairT m_ExpectedRd;
    RegPairT m_InitialRs1;
    RegPairT m_InitialRs2;
}; // class HartRTypeInstTest

class HartITypeInstTest : public HartSingleInstTestBase<HartITypeInstTest> {
public:
    constexpr HartITypeInstTest(std::string_view name, cpu::Opcode op, cpu::Function func, RegPairT rd, RegPairT rs1, Word imm) noexcept :
        HartSingleInstTestBase(cpu::Instruction(cpu::EncodeITypeInstruction(op, func, GetPairId(rd), GetPairId(rs1), imm)), name),
        m_ExpectedRd(rd),
        m_InitialRs1(rs1) {}
private:
    friend class HartSingleInstTestBase<HartITypeInstTest>;
    Result Initialize(HartTestSystem* pSys) const;
    Result Check(HartTestSystem* pSys) const;
private:
    RegPairT m_ExpectedRd;
    RegPairT m_InitialRs1;
}; // class HartITypeInstTest

} // namespace test
} // namespace riscv