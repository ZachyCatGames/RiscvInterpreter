#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <compare>

namespace riscv {
namespace cpu {

class TrapCode {
public:
    constexpr explicit operator NativeWord() const noexcept { return m_Code; }

    constexpr bool IsInterrupt() const noexcept {
        return m_Code >> (NativeWordBitLen - 1);
    }

    constexpr bool operator==(const TrapCode& rhs) const noexcept {
        return this->m_Code == rhs.m_Code;
    }
    
    constexpr auto operator<=>(const TrapCode& rhs) const noexcept {
        return this->m_Code <=> rhs.m_Code;
    }
public:
    static constexpr TrapCode CreateException(NativeWord val) noexcept {
        return TrapCode(val);
    }

    static constexpr TrapCode CreateInterrupt(NativeWord val) noexcept {
        return TrapCode(val | (static_cast<NativeWord>(val) << (NativeWordBitLen - 1)));
    }
private:
    constexpr TrapCode(NativeWord val) noexcept : m_Code(val) {}
private:
    NativeWord m_Code;
};

/** Traps defined by the riscv standard. */
struct BuiltinTrap {
    /* In-use interrupt IDs. */
    static constexpr auto IntSvSw       = TrapCode::CreateInterrupt( 1);
    static constexpr auto IntMSw        = TrapCode::CreateInterrupt( 3);
    static constexpr auto IntSvTimer    = TrapCode::CreateInterrupt( 5);
    static constexpr auto IntMTimer     = TrapCode::CreateInterrupt( 7);
    static constexpr auto IntSvExternal = TrapCode::CreateInterrupt( 9);
    static constexpr auto IntMExternal  = TrapCode::CreateInterrupt(11);

    /* Custom interrupt IDs start. */
    static constexpr auto IntCustomStart = TrapCode::CreateInterrupt(16);

    /* Reserved interrupt IDs. */
    static constexpr auto IntReserved0  = TrapCode::CreateInterrupt( 0);
    static constexpr auto IntReserved2  = TrapCode::CreateInterrupt( 2);
    static constexpr auto IntReserved4  = TrapCode::CreateInterrupt( 4);
    static constexpr auto IntReserved6  = TrapCode::CreateInterrupt( 6);
    static constexpr auto IntReserved8  = TrapCode::CreateInterrupt( 8);
    static constexpr auto IntReserved10 = TrapCode::CreateInterrupt(10);
    static constexpr auto IntReserved12 = TrapCode::CreateInterrupt(12);
    static constexpr auto IntReserved13 = TrapCode::CreateInterrupt(13);
    static constexpr auto IntReserved14 = TrapCode::CreateInterrupt(14);
    static constexpr auto IntReserved15 = TrapCode::CreateInterrupt(15);

    /* In-use exception IDs. */
    static constexpr auto ExcInstBadAlign   = TrapCode::CreateException(0);
    static constexpr auto ExcInstAccFault   = TrapCode::CreateException(1);
    static constexpr auto ExcInstIllegal    = TrapCode::CreateException(2);
    static constexpr auto ExcBreakpoint     = TrapCode::CreateException(3);
    static constexpr auto ExcLoadBadAlign   = TrapCode::CreateException(4);
    static constexpr auto ExcLoadAccFault   = TrapCode::CreateException(5);
    static constexpr auto ExcStoreBadAlign  = TrapCode::CreateException(6);
    static constexpr auto ExcStoreAccFault  = TrapCode::CreateException(7);
    static constexpr auto ExcEnvCallFromU   = TrapCode::CreateException(8);
    static constexpr auto ExcEnvCallFromS   = TrapCode::CreateException(9);
    static constexpr auto ExcEnvCallFromM   = TrapCode::CreateException(11);
    static constexpr auto ExcInstPageFault  = TrapCode::CreateException(12);
    static constexpr auto ExcLoadPageFault  = TrapCode::CreateException(13);
    static constexpr auto ExcStorePageFault = TrapCode::CreateException(15);

    /* Custom exception IDs. */
    static constexpr auto ExcCustomStart24 = TrapCode::CreateException(24);
    static constexpr auto ExcCustomEnd31   = TrapCode::CreateException(31);
    static constexpr auto ExcCustomStart48 = TrapCode::CreateException(48);
    static constexpr auto ExcCustomEnd63   = TrapCode::CreateException(63);

    /* Reserved exception IDs. */
    static constexpr auto ExcReserved10      = TrapCode::CreateException(10);
    static constexpr auto ExcReserved14      = TrapCode::CreateException(14);
    static constexpr auto ExcReservedStart16 = TrapCode::CreateException(16);
    static constexpr auto ExcReservedEnd23   = TrapCode::CreateException(23);
    static constexpr auto ExcReservedStart32 = TrapCode::CreateException(32);
    static constexpr auto ExcReservedEnd47   = TrapCode::CreateException(47);
    static constexpr auto ExcReservedStart64 = TrapCode::CreateException(64);
};

} // namespace cpu
} // namespace riscv
