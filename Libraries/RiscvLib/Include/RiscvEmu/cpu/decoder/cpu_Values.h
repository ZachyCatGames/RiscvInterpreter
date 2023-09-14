#pragma once
#include <RiscvEmu/cfg/cpu/cpu_Extensions.h>

namespace riscv {
namespace cpu {

constexpr inline auto ShiftAmtMaskFor32 = 0x1Fu;
constexpr inline auto ShiftAmtMaskFor64 = 0x3Fu;
constexpr inline auto ShiftAmtMask = cfg::cpu::EnableIsaRV64I ? 0x3Fu : 0x1Fu; 

} // namespace cpu
} // namespace riscv
