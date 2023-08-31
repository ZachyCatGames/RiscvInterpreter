#pragma once

namespace riscv {
namespace cpu {

enum class PrivilageLevel {
    User = 0,
    Supervisor = 1,
    Hypervisor = 2,
    Machine = 3
}; // enum class PrivilageLevel

} // namespace cpu
} // namespace riscv
