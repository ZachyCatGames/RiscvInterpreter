#pragma once

namespace riscv {
namespace cpu {

enum class CsrId {
    sscratch = 0x140,
    satp     = 0x180,

    mscratch = 0x340,

    time  = 0xC01,
    timeh = 0xC81,
}; // enum class CsrId

} // namespace cpu
} // namespace riscv
