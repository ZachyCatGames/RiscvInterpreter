#pragma once

namespace riscv {
namespace cpu {

enum class PrivilageLevel {
    User = 0,
    Supervisor = 1,
    Hypervisor = 2,
    Machine = 3
}; // enum class PrivilageLevel

enum class AddrTransMode {
    Bare = 0,
    Sv32 = 1,
    Sv39 = 8,
    Sv48 = 9,
    Sv57 = 10
}; // enum class AddrTransMode

} // namespace cpu
} // namespace riscv
