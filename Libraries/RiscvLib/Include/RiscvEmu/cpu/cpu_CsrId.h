#pragma once

namespace riscv {
namespace cpu {

enum class CsrId {
    sscratch = 0x140,
    satp     = 0x180,

    misa = 0x301,

    mscratch = 0x340,

    mcycle    = 0xB00,
    minstret  = 0xB02,
    mcycleh   = 0xB80,
    minstreth = 0xB82,

    cycle    = 0xC00,
    time     = 0xC01,
    instret  = 0xC02,
    cycleh   = 0xC80,
    timeh    = 0xC81,
    instreth = 0xC82
}; // enum class CsrId

} // namespace cpu
} // namespace riscv
