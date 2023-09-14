#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>
#include <RiscvEmuTest/test_TestCaseBase.h>
#include <RiscvEmuTest/cpu/test_HartTestSystem.h>

namespace riscv {
namespace test {

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
        if(res.IsFailure()) {
            return res;
        }

        /* Perform check. */
        return static_cast<const Derived*>(this)->Check(pSys);
    }
protected:
    constexpr HartSingleInstTestBase(cpu::Instruction inst, std::string_view name) noexcept :
        TestCaseBase<HartSingleInstTestBase<Derived>, HartTestSystem>(name),
        m_Inst(inst) {}

    using RegPairT = std::pair<int, NativeWord>;

    static constexpr auto GetPairId(RegPairT pair) noexcept { return std::get<0>(pair); }
    static constexpr auto GetPairVal(RegPairT pair) noexcept { return std::get<1>(pair); }    
private:
    cpu::Instruction m_Inst;
}; // class HartSingleInstTestBase

} // namespace test
} // namespace riscv