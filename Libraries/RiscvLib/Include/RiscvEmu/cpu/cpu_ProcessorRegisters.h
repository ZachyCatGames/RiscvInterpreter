#pragma once
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace cpu {

struct ProcessorRegisters {
    static constexpr auto GprCount = cfg::cpu::EnableIsaRV32E ? 16 : 32;

    NativeWordU x[GprCount];

    Address pc;
};

} // namespace cpu
} // namespace riscv
