#pragma once

namespace riscv {
namespace util {

template<typename T>
constexpr auto BitSwap(T val) {
    constexpr std::size_t Size = sizeof(val) * 8;

    T result = 0;
    for (std::size_t i = 0; i < Size; i++) {
        result |= (val & (1u << i)) >> (Size - i - 1);
    }

    return result;
}

} // namespace util
} // namespace riscv
