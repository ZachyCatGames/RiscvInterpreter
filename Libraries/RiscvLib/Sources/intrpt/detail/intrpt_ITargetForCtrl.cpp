#include <RiscvEmu/intrpt/detail/intrpt_ITargetForCtrl.h>
#include <RiscvEmu/intrpt/intrpt_ITarget.h>
#include <RiscvEmu/diag.h>
#include <memory>

namespace riscv {
namespace intrpt {
namespace detail {

ITargetForCtrl::ITargetForCtrl() :
    m_pTarget(nullptr) {}

ITargetForCtrl::ITargetForCtrl(std::shared_ptr<ITarget>&& pTarget) { this->Initialize(std::move(pTarget)); }

ITargetForCtrl::~ITargetForCtrl() = default;

void ITargetForCtrl::Initialize(std::shared_ptr<ITarget>&& pTarget) {
    diag::Assert(!this->IsInitialized(), "Should not be initialized");
    m_pTarget = pTarget;
}

} // namespace detail
} // namespace intrpt
} // namespace riscv
