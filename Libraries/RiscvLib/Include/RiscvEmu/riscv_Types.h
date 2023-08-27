#pragma once
#include <RiscvEmu/cfg/cpu/cpu_Extensions.h>
#include <cstdint>
#include <type_traits>

namespace riscv {

/** Unsigned 8bit Byte. */
using Byte = uint8_t;

/** Signed 8bit Byte. */
using ByteS = int8_t;

/** Unsigned 16bit Half Word. */
using HWord = uint16_t;

/** Signed 16bit Half Word. */
using HWordS = int16_t;

/** Unsigned 32bit Word. */
using Word = uint32_t;

/** Signed 32bit Word. */
using WordS = int32_t;

/** Unsigned 64bit Double Word. */
using DWord = uint64_t;

/** Signed 64bit Double Word. */
using DWordS = int64_t;

/** Unsigned native word (WordU for RV32I/E, DWordU for RV64I). */
using NativeWord = std::conditional_t<cfg::cpu::EnableIsaRV64I, DWord, Word>;

/** Signed native word type (WordS for RV32I/E, DWordS for RV64I). */
using NativeWordS = std::conditional_t<cfg::cpu::EnableIsaRV64I, DWordS, WordS>;

/** Address type, same as NativeWordU. */
using Address = NativeWord;

/** Byte Length. */
constexpr inline auto ByteLen = sizeof(Byte);

/* Byte Length in Bits. */
constexpr inline auto ByteBitLen = ByteLen * 8;

/** Half Word Length. */
constexpr inline auto HWordLen = sizeof(HWord);

/** Half Word Length in Bits. */
constexpr inline auto HWordBitLen = HWordLen * 8;

/** Word Length. */
constexpr inline auto WordLen = sizeof(Word);

/** Word Length in Bits. */
constexpr inline auto WordBitLen = WordLen * 8;

/** Double Word Length. */
constexpr inline auto DWordLen = sizeof(DWord);

/** Double Word Length in Bits. */
constexpr inline auto DWordBitLen = DWordLen * 8;

/** Native Word Length. */
constexpr inline auto NativeWordLen = sizeof(NativeWord);

/** Native Word Length in Bits. */
constexpr inline auto NativeWordBitLen = NativeWordLen * 8;

/** Address Length. */
constexpr inline auto AddressLen = sizeof(Address);

/** Address Length in Bits. */
constexpr inline auto AddressBitLen = AddressLen * 8;

} // namespace riscv
