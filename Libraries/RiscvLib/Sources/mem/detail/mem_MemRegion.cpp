#include <RiscvEmu/mem/detail/mem_MemRegion.h>

namespace riscv {
namespace mem {
namespace detail {

MemRegion::MemRegion() noexcept = default;

void MemRegion::Initialize(const RegionBase& info) {
    RegionBase::Initialize(info);
    m_Dev.Initialize(info.GetLength());
}

} // namespace detail
} // namespace mem
} // namespace riscv
