#pragma once
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace result {
namespace detail {

constexpr inline WordU SuccessValue = 0;

constexpr WordU CreateResultValue(WordU modId, WordU desc) noexcept {
    return modId << 24 | (desc & 0xFFFFFF);
}

constexpr WordU GetModuleIdFromValue(WordU val) noexcept {
    return val >> 24;
}

constexpr WordU GetDescriptionFromValue(WordU val) noexcept {
    return val & 0xFFFFFF;
}

} // namespace detail
} // namespace result
} // namespace riscv
