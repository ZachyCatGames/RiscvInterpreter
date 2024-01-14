#pragma once
#include <RiscvEmu/result.h>
#include <memory>

namespace riscv {
namespace intrpt {

class ITarget;

namespace detail {

class ITargetForCtrl {
public:
    ITargetForCtrl();
    ITargetForCtrl(std::shared_ptr<ITarget>&& pTarget);
    virtual ~ITargetForCtrl();

    /**
     * Initialize.
    */
    void Initialize(std::shared_ptr<ITarget>&& pTarget);

    void Finalize();

    bool IsInitialized() const noexcept;

    Result NotifyAvailableIRQ();

    /**
     * Returns whether an interrupt is currently pending.
    */
    virtual bool HasPendingIRQ() = 0;

    /**
     * Notify the interrupt controller that the target won't be accepting
     * interrupt requests.
    */
    virtual Result EnableInterrupts() = 0;

    /**
     * Notify the interrupt controller that the target will be accepting
     * interrupt requests.
    */
    virtual Result DisableInterrupts() = 0;
private:
    std::shared_ptr<ITarget> m_pTarget;
}; // class ITargetCtlrBridge

} // namespace detail
} // namespace intrpt
} // namespace riscv