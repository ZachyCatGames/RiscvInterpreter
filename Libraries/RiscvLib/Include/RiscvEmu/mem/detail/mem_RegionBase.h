#pragma once
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace mem {
namespace detail {

class RegionBase {
public:
    constexpr auto GetStart() const noexcept { return m_Address; }

    constexpr auto GetEnd() const noexcept { return m_Address + m_Length; }

    constexpr auto GetLength() const noexcept { return m_Length; }

    constexpr auto Includes(Address addr) const noexcept {
        return addr >= this->GetStart() && addr <= this->GetEnd();
    }
protected:
    constexpr RegionBase(Address addr, NativeWord len) :
        m_Address(addr),
        m_Length(len) {}

    constexpr RegionBase(const RegionBase& other) noexcept = default;

    constexpr RegionBase& operator=(const RegionBase& rhs) noexcept {
        this->m_Address = rhs.m_Address;
        this->m_Length = rhs.m_Length;
        return *this;
    }

    constexpr void Initialize(Address addr, NativeWord len) noexcept {
        m_Address = addr;
        m_Length = len;
    }

    constexpr void Initialize(const RegionBase& other) noexcept {
        m_Address = other.m_Address;
        m_Length = other.m_Length;
    }
private:
    Address m_Address;
    NativeWord m_Length;
}; // class RegionBase

} // namespace detail
} // namespace riscv
} // namespace mem
