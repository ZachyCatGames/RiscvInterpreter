#pragma once

namespace riscv {
namespace cpu {

enum class MemAccessReason {
    Read,
    Write,
    Instruction,
    Any
}; // enum class MemAccessReason

} // namespace cpu
} // namespace riscv
