#pragma once
#include <RiscvEmu/util/util_Bitmask.h>
#include <concepts>
#include <utility>

namespace riscv {
namespace util {

template<std::unsigned_integral T>
class Bitfields {
public:
    constexpr Bitfields() noexcept : m_Value(0) {}
    explicit constexpr Bitfields(const T& val) noexcept : m_Value(val) {}
    explicit constexpr Bitfields(T&& val) noexcept : m_Value(std::move(val)) {}

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
constexpr auto ExtractBitfield(const T& val, int offset, int size) noexcept -> std::remove_cvref_t<T> {
    return Bitfields(val).GetField(offset, size);
}

template<typename T>
constexpr auto ExtractBitfield(const T& val, std::pair<int, int> pair) noexcept -> std::remove_cvref_t<T> {
    return Bitfields(val).GetField(pair);
}

template<typename T>
constexpr auto AssignBitfield(const T& target, int offset, int size, const T& val) noexcept -> std::remove_cvref_t<T> {
    return Bitfields(target).SetField(offset, size, val).GetValue();
}

template<typename T>
constexpr auto AssignBitfield(const T& target, std::pair<int, int> pair, const T& val) noexcept -> std::remove_cvref_t<T> {
    return Bitfields(target).SetField(pair, val).GetValue();
}

template<typename T>
constexpr bool ExtractBitArray(const T* pTarget, int offset) noexcept {
    return pTarget[offset / (sizeof(T) * 8)] >> (offset % (sizeof(T) * 8));
}

template<typename T>
constexpr void AssignBitArray(T* pTarget, int offset, bool val) noexcept {
    auto index = offset / (sizeof(T) * 8);
    auto bitIndex = offset % (sizeof(T) * 8);
    auto mask = ~(1 << bitIndex);

    pTarget[index] = (pTarget[index] & mask) | (static_cast<T>(val) << bitIndex);
}

} // namespace util
} // namespace riscv
