#pragma once
#include <Riscv/util/util_Bitmask.h>
#include <type_traits>

namespace riscv {
namespace util {

template<std::size_t From, std::size_t To, typename T>
constexpr auto SignExtend(T val) {
    static constexpr T ExtendMask = util::GenerateMaskLeft<T>(To - From);
    static constexpr T SignCheck = 1 << (From - 1);

    if (val & SignCheck) {
        val |= ExtendMask;
    }

    return val;
}

template<typename From, typename To>
constexpr auto SignExtendType(From val) {
    return static_cast<To>(static_cast<std::make_signed_t<From>>(val));
}

} // namespace util
} // namespace riscv
