#pragma once
#include <RiscvEmu/cpu/cpu_Hart.h>

namespace riscv {
namespace cpu {

Result Hart::Initialize(SharedContext* pSharedCtx) {
    m_pSharedCtx = pSharedCtx;

    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
