#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/diag.h>

namespace riscv {
namespace cpu {

void Hart::SharedState::Initialize(Word hartCount) noexcept {
    m_HartCount = hartCount;
}

} // namespace cpu
} // namespace riscv