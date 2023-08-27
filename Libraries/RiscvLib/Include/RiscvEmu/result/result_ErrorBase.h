#pragma once
#include <RiscvEmu/result/result_Result.h>

namespace riscv {
namespace result {

template<auto ModId, auto Desc>
class ErrorBase {
public:
    constexpr auto GetValue() const noexcept { return Value; }

    constexpr auto IsSuccess() const noexcept { return false; }
private:
    static constexpr auto Value = detail::CreateResultValue(ModId, Desc);
}; // class ErrorBase

} // namespace result
} // namespace riscv
