#pragma once
#include <cstddef>

namespace riscv {
namespace util {

template<typename T>
constexpr bool IsAligned(const T& val, std::size_t align) {
    return val % align == 0;
}

} // namespace util
} // namespace riscv
