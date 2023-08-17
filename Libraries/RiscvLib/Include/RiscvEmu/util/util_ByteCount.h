#pragma once

namespace riscv {
namespace util {

template<typename T, typename S>
constexpr T KibiBytes(S count) {
    return static_cast<T>(count * 1024);
}

template<typename T, typename S>
constexpr T MibiBytes(S count) {
    return static_cast<T>(KibiBytes(count) * 1024);
}

template<typename T, typename S>
constexpr T GibiBytes(S count) {
    return static_cast<T>(MibiBytes(count) * 1024);
}

} // namespace util
} // namespace riscv
