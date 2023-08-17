#pragma once

namespace riscv {
namespace util {

template<typename T>
constexpr bool CheckAddOverflow(T result, T val1, T val2) {
    return (val1 > 0 && val2 > 0 && result < 0) || (val1 < 0 && val2 < 0 && result < 0);
}

template<typename T>
constexpr bool CheckSubOverflow(T result, T val1, T val2) {
    return (val1 > 0 && val2 < 0 && result < 0) || (val1 < 0 && val2 > 0 && result < 0);
}

} // namespace util
} // namespace riscv
