#pragma once

namespace riscv {
namespace cpu {

enum class MemAccessReason {
    Read,
    Write,
    Instruction
}; // enum class MemAccessReason

} // namespace cpu
} // namespace riscv
