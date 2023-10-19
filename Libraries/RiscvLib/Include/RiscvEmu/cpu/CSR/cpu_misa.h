#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Types.h>
#include <RiscvEmu/util/util_Bitfields.h>
#include <type_traits>

namespace riscv {
namespace cpu {
namespace csr {

namespace detail {

class misaBase : protected util::Bitfields<NativeWord> {
public:
    constexpr bool GetA() const noexcept { return this->GetField(0, 1); }
    constexpr void SetA(bool val) noexcept { this->SetField(0, 1, val); }

    constexpr bool GetB() const noexcept { return this->GetField(1, 1); }
    constexpr void SetB(bool val) noexcept { this->SetField(1, 1, val); }

    constexpr bool GetC() const noexcept { return this->GetField(2, 1); }
    constexpr void SetC(bool val) noexcept { this->SetField(2, 1, val); }

    constexpr bool GetD() const noexcept { return this->GetField(3, 1); }
    constexpr void SetD(bool val) noexcept { this->SetField(3, 1, val); }

    constexpr bool GetE() const noexcept { return this->GetField(4, 1); }
    constexpr void SetE(bool val) noexcept { this->SetField(4, 1, val); }

    constexpr bool GetF() const noexcept { return this->GetField(5, 1); }
    constexpr void SetF(bool val) noexcept { this->SetField(5, 1, val); }

    constexpr bool GetG() const noexcept { return this->GetField(6, 1); }
    constexpr void SetG(bool val) noexcept { this->SetField(6, 1, val); }

    constexpr bool GetH() const noexcept { return this->GetField(7, 1); }
    constexpr void SetH(bool val) noexcept { this->SetField(7, 1, val); }

    constexpr bool GetI() const noexcept { return this->GetField(8, 1); }
    constexpr void SetI(bool val) noexcept { this->SetField(8, 1, val); }

    constexpr bool GetJ() const noexcept { return this->GetField(9, 1); }
    constexpr void SetJ(bool val) noexcept { this->SetField(9, 1, val); }

    constexpr bool GetK() const noexcept { return this->GetField(10, 1); }
    constexpr void SetK(bool val) noexcept { this->SetField(10, 1, val); }

    constexpr bool GetL() const noexcept { return this->GetField(11, 1); }
    constexpr void SetL(bool val) noexcept { this->SetField(11, 1, val); }

    constexpr bool GetM() const noexcept { return this->GetField(12, 1); }
    constexpr void SetM(bool val) noexcept { this->SetField(12, 1, val); }

    constexpr bool GetN() const noexcept { return this->GetField(13, 1); }
    constexpr void SetN(bool val) noexcept { this->SetField(13, 1, val); }

    constexpr bool GetO() const noexcept { return this->GetField(14, 1); }
    constexpr void SetO(bool val) noexcept { this->SetField(14, 1, val); }

    constexpr bool GetP() const noexcept { return this->GetField(15, 1); }
    constexpr void SetP(bool val) noexcept { this->SetField(15, 1, val); }

    constexpr bool GetQ() const noexcept { return this->GetField(16, 1); }
    constexpr void SetQ(bool val) noexcept { this->SetField(16, 1, val); }

    constexpr bool GetR() const noexcept { return this->GetField(17, 1); }
    constexpr void SetR(bool val) noexcept { this->SetField(17, 1, val); }

    constexpr bool GetS() const noexcept { return this->GetField(18, 1); }
    constexpr void SetS(bool val) noexcept { this->SetField(18, 1, val); }

    constexpr bool GetT() const noexcept { return this->GetField(19, 1); }
    constexpr void SetT(bool val) noexcept { this->SetField(19, 1, val); }

    constexpr bool GetU() const noexcept { return this->GetField(20, 1); }
    constexpr void SetU(bool val) noexcept { this->SetField(20, 1, val); }

    constexpr bool GetV() const noexcept { return this->GetField(21, 1); }
    constexpr void SetV(bool val) noexcept { this->SetField(21, 1, val); }

    constexpr bool GetW() const noexcept { return this->GetField(22, 1); }
    constexpr void SetW(bool val) noexcept { this->SetField(22, 1, val); }

    constexpr bool GetX() const noexcept { return this->GetField(23, 1); }
    constexpr void SetX(bool val) noexcept { this->SetField(23, 1, val); }

    constexpr bool GetY() const noexcept { return this->GetField(24, 1); }
    constexpr void SetY(bool val) noexcept { this->SetField(24, 1, val); }

    constexpr bool GetZ() const noexcept { return this->GetField(25, 1); }
    constexpr void SetZ(bool val) noexcept { this->SetField(25, 1, val); }

    constexpr NativeWord GetValue() const noexcept { return util::Bitfields<NativeWord>::GetValue(); }
}; // class misaBase

class misaFor32 : public misaBase {
public:
    constexpr int GetMXL() const noexcept { return static_cast<int>(this->GetField(30, 2)); }
    constexpr void SetMXL(int val) noexcept { this->SetField(30, 2, static_cast<NativeWord>(val)); }
}; // class misaFor32

class misaFor64 : public misaBase {
public:
    constexpr int GetMXL() const noexcept { return static_cast<int>(this->GetField(62, 2)); }
    constexpr void SetMXL(int val) noexcept { this->SetField(30, 2, static_cast<NativeWord>(val)); }
}; // class misaFor64

} // namespace detail

class misa : public std::conditional_t<cfg::cpu::EnableIsaRV64I, detail::misaFor64, detail::misaFor32> {};

} // namespace csr
} // namespace cpu
} // namespace riscv
