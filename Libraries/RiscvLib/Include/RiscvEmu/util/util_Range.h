#pragma once

namespace riscv {
namespace util {

template<typename T>
constexpr bool InRange(const T& val, const T& min, const T& max) {
    return val >= min && val <= max;
}

} // namespace util
} // namespace riscv
