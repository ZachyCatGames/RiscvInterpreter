#pragma once
#include <RiscvEmu/util/util_Bitmask.h>

namespace riscv {
namespace util {

template<typename T>
constexpr auto SignExtend(T val, int from, int to) {
    /* Check if value is signed. */
    if(from < to && val & 1 << (from - 1)) {
        val |= util::GenerateMaskLeft<T>(to - from);
    }

    return val;
}

} // namespace util
} // namespace riscv
