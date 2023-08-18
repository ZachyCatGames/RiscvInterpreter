#pragma once
#include <RiscvEmu/cfg/cpu/cpu_Extensions.h>
#include <cstdint>
#include <type_traits>

namespace riscv {

/** Unsigned 8bit Byte. */
using ByteU = uint8_t;

/** Signed 8bit Byte. */
using ByteS = int8_t;

/** Unsigned 16bit Half Word. */
using HWordU = uint16_t;

/** Signed 16bit Half Word. */
using HWordS = int16_t;

/** Unsigned 32bit Word. */
using WordU = uint32_t;

/** Signed 32bit Word. */
using WordS = int32_t;

/** Unsigned 64bit Double Word. */
using DWordU = uint64_t;

/** Signed 64bit Double Word. */
using DWordS = int64_t;

/** Unsigned native word (WordU for RV32I/E, DWordU for RV64I). */
using NativeWordU = std::conditional_t<cfg::cpu::EnableIsaRV64I, DWordU, WordU>;

/** Signed native word type (WordS for RV32I/E, DWordS for RV64I). */
using NativeWordS = std::conditional_t<cfg::cpu::EnableIsaRV64I, DWordS, WordS>;

/** Address type, same as NativeWordU. */
using Address = NativeWordU;

} // namespace riscv
