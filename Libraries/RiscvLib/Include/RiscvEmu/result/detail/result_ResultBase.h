#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/result/detail/result_ResultValue.h>

namespace riscv {
namespace result {
namespace detail {

template<typename Derived>
class ResultBase {
public:
    constexpr auto GetModuleId() const noexcept {
        return GetModuleIdFromValue(static_cast<const Derived*>(this)->GetValue());
    }

    constexpr auto GetDescription() const noexcept {
        return GetDescriptionFromValue(static_cast<const Derived*>(this)->GetValue());
    }

    constexpr auto IsFailure() const noexcept {
        return !static_cast<const Derived*>(this)->IsSuccess();
    }
}; // class ResultBase

} // namespace detail
} // namespace result
} // namespace riscv
