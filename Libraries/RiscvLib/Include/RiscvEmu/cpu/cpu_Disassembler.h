#pragma once
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>
#include <string>

namespace riscv {
namespace cpu {

std::string DisassembleInstruction(Instruction inst);

} // namespace cpu
} // namespace riscv
