#pragma once
#include <RiscvEmu/cpu/cpu_ProcessorRegisters.h>
#include <RiscvEmu/cpu/cpu_Instruction.h>

namespace riscv {
namespace cpu {

class ProcessorCore {
public:
    ProcessorCore() = default;

    Result ExecuteInstruction(Instruction inst);

    Result SetPc(Address addr);

    Result ExecuteAtPc();

    ProcessorRegisters* GetRegisterContext() const noexcept;
private:
    ProcessorRegisters m_Registers;
};

} // namespace cpu
} // namespace riscv
