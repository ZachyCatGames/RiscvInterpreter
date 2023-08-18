#pragma once

namespace riscv {
namespace cpu {

enum PrivilageLevel {
    PrivilageLevel_User = 0,
    PrivilageLevel_Supervisor = 1,
    PrivilageLevel_Hypervisor = 2,
    PrivilageLevel_Machine = 3
};

} // namespace cpu
} // namespace riscv
