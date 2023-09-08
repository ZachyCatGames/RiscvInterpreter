#pragma once
#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::Initialize(SharedContext sharedCtx) {
    m_SharedCtx = sharedCtx;

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
