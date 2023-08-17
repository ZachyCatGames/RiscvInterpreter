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
    constexpr Bitfields() : m_Value(0) {}
    explicit constexpr Bitfields(const T& val) : m_Value(val) {}

    constexpr const T& GetValue() const {
        return m_Value;
    }
    
    constexpr void SetValue(const T& value) {
        m_Value = value;
    }

    constexpr auto& SetField(int offset, int size, const T& value) {
        T Mask = GenerateMaskRight<T>(size);

        /* Clear bits */
        m_Value &= ~(Mask << offset);

        /* Sets bits to new value. */
        m_Value |= (value & Mask) << offset;

        return *this;
    }

    constexpr auto& SetField(std::pair<int, int> pair, const T& value) {
        return this->SetField(std::get<0>(pair), std::get<1>(pair), value);
    }
    
    constexpr T GetField(int offset, int size) const {
        return m_Value >> offset & GenerateMaskRight<T>(size);
    }

    constexpr T GetField(std::pair<int, int> pair) const {
        return this->GetField(std::get<0>(pair), std::get<1>(pair));
    }

private:
    T m_Value;
};

} // namespace util
} // namespace riscv
