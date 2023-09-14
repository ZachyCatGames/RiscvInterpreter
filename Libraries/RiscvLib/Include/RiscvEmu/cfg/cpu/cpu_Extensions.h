#pragma once

namespace riscv {
namespace cfg {
namespace cpu {

constexpr inline bool EnableIsaRV32E = false;

#ifdef RISCV_CFG_CPU_ENABLE_RV64
constexpr inline bool EnableIsaRV64I = true;
#else
constexpr inline bool EnableIsaRV64I = false;
#endif

} // namespace cpu
} // namespace cfg
} // namespace riscv
