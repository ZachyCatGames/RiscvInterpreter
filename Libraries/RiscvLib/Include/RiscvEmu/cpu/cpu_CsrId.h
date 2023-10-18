#pragma once

namespace riscv {
namespace cpu {

enum class CsrId {
    sscratch = 0x140,
    satp     = 0x180
}; // enum class CsrId

} // namespace cpu
} // namespace riscv