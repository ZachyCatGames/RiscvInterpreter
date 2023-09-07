#pragma once
#include <cstdint>
#include <limits>

namespace riscv {
namespace util {

template<typename T>
constexpr auto GenerateMaskRight(int size) noexcept {
    return std::numeric_limits<T>::max() >> ((sizeof(T) * 8) - static_cast<T>(size));
}

static_assert(GenerateMaskRight<uint64_t>(16) == 0xFFFFULL);
static_assert(GenerateMaskRight<uint32_t>(7) == 0x7FUL);

template<typename T>
constexpr auto GenerateMaskLeft(int size) noexcept {
    return std::numeric_limits<T>::max() << ((sizeof(T) * 8) - static_cast<T>(size));
}

static_assert(GenerateMaskLeft<uint64_t>(16) == 0xFFFF000000000000ULL);
static_assert(GenerateMaskLeft<uint32_t>(7) == 0xFE000000UL);

} // namespace util
} // namespace riscv
