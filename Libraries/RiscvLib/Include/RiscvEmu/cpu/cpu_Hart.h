#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Instruction.h>
#include <RiscvEmu/cpu/detail/cpu_SharedHartContext.h>
#include <cassert>

namespace riscv {
namespace cpu {

class Hart {
public:
    Hart(detail::SharedHartContext* pSharedCtx) :
        m_pSharedCtx(pSharedCtx) {}

    Result ExecuteInstruction(Instruction inst);

    void WritePC(NativeWord addr) noexcept { m_PC = addr; }
    void WriteGPR(int index, NativeWord value) noexcept {
        assert(index < c_NumGPR && index >= 0);
        m_GPR[index] = value;
    }

    NativeWord ReadPC() const noexcept { return m_PC; }
    NativeWord ReadGPR(int index) const noexcept {
        assert(index < c_NumGPR && index >= 0);
        return m_GPR[index];
    }

    Result ExecuteAtPc();

    Result Reset();
private:
    static constexpr auto c_NumGPR = cfg::cpu::EnableIsaRV32E ? 16 : 32;
    NativeWord m_PC;
    NativeWord m_GPR[c_NumGPR];

    detail::SharedHartContext* const m_pSharedCtx;
}; // class Hart

} // namespace cpu
} // namespace riscv
