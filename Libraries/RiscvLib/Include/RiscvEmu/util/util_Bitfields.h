#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>
#include <concepts>
#include <Riscv/util/util_Bitmask.h>

namespace riscv {
namespace util {

template<std::unsigned_integral T>
class Bitfields {
public:
    constexpr Bitfields() noexcept : m_Value(0) {}
    explicit constexpr Bitfields(const T& val) noexcept : m_Value(val) {}

    constexpr const T& GetValue() const noexcept {
        return m_Value;
    }
    
    constexpr void SetValue(const T& value) {
        m_Value = value;
    }

    constexpr auto& SetField(int offset, int size, const T& value) noexcept {
        T Mask = GenerateMaskRight<T>(size);

        /* Clear bits */
        m_Value &= ~(Mask << offset);

        /* Sets bits to new value. */
        m_Value |= (value & Mask) << offset;

        return *this;
    }

    constexpr auto& SetField(std::pair<int, int> pair, const T& value) noexcept {
        return this->SetField(std::get<0>(pair), std::get<1>(pair), value);
    }
    
    constexpr T GetField(int offset, int size) const noexcept {
        return m_Value >> offset & GenerateMaskRight<T>(size);
    }

    constexpr T GetField(std::pair<int, int> pair) const noexcept {
        return this->GetField(std::get<0>(pair), std::get<1>(pair));
    }

private:
    T m_Value;
};

template<typename T>
constexpr T ExtractBitfield(const T& val, int offset, int size) noexcept {
    return Bitfields(val).GetField(offset, size);
}

template<typename T>
constexpr T ExtractBitfield(const T& val, std::pair<int, int> pair) noexcept {
    return Bitfields(val).GetField(pair);
}

template<typename T>
constexpr T AssignBitfield(const T& val, int offset, int size, const T& val) noexcept {
    return Bitfields(val).SetField(offset, size, val).GetValue();
}

template<typename T>
constexpr T AssignBitfield(const T& val, std::pair<int, int> pair, const T& val) noexcept {
    return Bitfields(val).SetField(offset, pair, val).GetValue();
}

} // namespace util
} // namespace riscv
