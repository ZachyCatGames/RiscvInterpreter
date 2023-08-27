#pragma once
#include <RiscvEmu/result/detail/result_ResultBase.h>

namespace riscv {

class Result : public result::detail::ResultBase<Result> {
public:
    constexpr Result() noexcept : m_Value(0) {}
    constexpr Result(WordU val) noexcept : m_Value(val) {}

    /**
     * Returns whether a Result indicates success.
    */
    constexpr bool IsSuccess() const noexcept { return m_Value == result::detail::SuccessValue; }
private:
    WordU m_Value;
}; // class Result

} // namespace riscv
