#pragma once
#include <RiscvEmu/result/result_Result.h>

namespace riscv {

class ResultSuccess : public result::detail::ResultBase<ResultSuccess> {
public:
    constexpr auto GetValue() const noexcept { return result::detail::SuccessValue; }

    constexpr auto IsSuccess() const noexcept { return true; }

    constexpr operator Result() const noexcept { return Result(result::detail::SuccessValue); }

    constexpr bool Includes(Result res) const noexcept { return res.GetValue() == result::detail::SuccessValue; }
}; // class ResultSuccess

} // namespace riscv
