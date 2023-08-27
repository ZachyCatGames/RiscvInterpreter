#pragma once
#include <RiscvEmu/result/detail/result_ResultBase.h>

namespace riscv {

class Result : public result::detail::ResultBase<Result> {
public:
    constexpr Result() noexcept : m_Value(0) {}
    constexpr Result(Word val) noexcept : m_Value(val) {}

    constexpr auto GetValue() const noexcept { return m_Value; }

    /**
     * Returns whether a Result indicates success.
    */
    constexpr bool IsSuccess() const noexcept { return m_Value == result::detail::SuccessValue; }
private:
    Word m_Value;
}; // class Result

} // namespace riscv
