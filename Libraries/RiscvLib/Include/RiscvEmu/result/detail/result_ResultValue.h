#pragma once
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace result {
namespace detail {

constexpr inline Word SuccessValue = 0;

constexpr Word CreateResultValue(Word modId, Word desc) noexcept {
    return modId << 24 | (desc & 0xFFFFFF);
}

constexpr Word GetModuleIdFromValue(Word val) noexcept {
    return val >> 24;
}

constexpr Word GetDescriptionFromValue(Word val) noexcept {
    return val & 0xFFFFFF;
}

} // namespace detail
} // namespace result
} // namespace riscv
