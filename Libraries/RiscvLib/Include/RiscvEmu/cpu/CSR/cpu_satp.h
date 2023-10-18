#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Types.h>
#include <RiscvEmu/util/util_Bitfields.h>
#include <type_traits>

namespace riscv {
namespace cpu {
namespace csr {

namespace detail {

class satpFor32 : private util::Bitfields<NativeWord> {
public:
    constexpr satpFor32() noexcept : util::Bitfields<NativeWord>(0) {}
    constexpr satpFor32(NativeWord val) noexcept : util::Bitfields<NativeWord>(val) {}

    constexpr NativeWord GetPPN() const noexcept { return this->GetField(0, 22); }
    constexpr satpFor32& SetPPN(NativeWord val) noexcept { this->SetField(0, 22, val); return *this; }

    constexpr Word GetASID() const noexcept { return static_cast<Word>(this->GetField(22, 8)); }
    constexpr satpFor32& SetASID(Word val) noexcept { this->SetField(22, 8, static_cast<NativeWord>(val)); return *this; }

    constexpr AddrTransMode GetMODE() const noexcept { return static_cast<AddrTransMode>(this->GetField(31, 1 )); }
    constexpr satpFor32& SetMode(AddrTransMode mode) noexcept { this->SetField(31, 1, static_cast<NativeWord>(mode)); return *this; }

    constexpr NativeWord GetValue() const noexcept { return util::Bitfields<NativeWord>::GetValue(); }
}; // class satpFor32

class satpFor64 : private util::Bitfields<NativeWord> {
public:
    constexpr satpFor64() noexcept : util::Bitfields<NativeWord>(0) {}
    constexpr satpFor64(NativeWord val) noexcept : util::Bitfields<NativeWord>(val) {}

    constexpr NativeWord GetPPN() const noexcept { return this->GetField(0, 44); }
    constexpr satpFor64& SetPPN(NativeWord val) noexcept { this->SetField(0, 44, val); return *this; }

    constexpr Word GetASID() const noexcept { return static_cast<Word>(this->GetField(44, 16)); }
    constexpr satpFor64& SetASID(Word val) noexcept { this->SetField(44, 16, static_cast<NativeWord>(val)); return *this; }

    constexpr AddrTransMode GetMODE() const noexcept { return static_cast<AddrTransMode>(this->GetField(60, 4 )); }
    constexpr satpFor64& SetMODE(AddrTransMode mode) noexcept { this->SetField(60, 4, static_cast<NativeWord>(mode)); return *this; }

    constexpr NativeWord GetValue() const noexcept { return util::Bitfields<NativeWord>::GetValue(); }
}; // class satpFor64

} // namespace detail

class satp : public std::conditional_t<cfg::cpu::EnableIsaRV64I, detail::satpFor64, detail::satpFor32> {};

} // namespace csr
} // namespace cpu
} // namespace riscv
