#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Opcodes.h>

namespace riscv {
namespace cpu {

namespace detail {

constexpr Word EncodeSBTypeCommon(Opcode op, int f3, int rs1, int rs2) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(f3)  & 0x07u)  << 12 |
            (static_cast<Word>(rs1) & 0x3Fu)  << 15 |
            (static_cast<Word>(rs2) & 0x3Fu)  << 20;
}

constexpr Word EncodeUJTypeCommon(Opcode op, int rd) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(rd)  & 0x3Fu) << 7;
}

} // namespace detal

constexpr Word EncodeRTypeInstruction(Opcode op, int f3, int f7, int rd, int rs1, int rs2) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(rd)  & 0x3Fu) << 7 |
            (static_cast<Word>(f3)  & 0x07u) << 12 |
            (static_cast<Word>(rs1) & 0x3Fu) << 15 |
            (static_cast<Word>(rs2) & 0x3Fu) << 20 |
            (static_cast<Word>(f7)  & 0x7Fu) << 25;
}

constexpr Word EncodeRTypeInstruction(Opcode op, Function func, int rd, int rs1, int rs2) noexcept {
    return EncodeRTypeInstruction(op, GetFunction3(func), GetFunction7(func), rd, rs1, rs2);
}

constexpr Word EncodeITypeInstruction(Opcode op, int f3, int rd, int rs1, Word imm) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(rd)  & 0x3Fu) << 7 |
            (static_cast<Word>(f3)  & 0x07u) << 12 |
            (static_cast<Word>(rs1) & 0x3Fu) << 15 |
            (static_cast<Word>(imm))         << 20;
}

constexpr Word EncodeITypeInstruction(Opcode op, Function func, int rd, int rs1, Word imm) noexcept {
    return EncodeITypeInstruction(op, static_cast<int>(func), rd, rs1, imm);
}

constexpr Word EncodeSTypeInstruction(Opcode op, int f3, int rs1, int rs2, Word imm) noexcept {
    return  detail::EncodeSBTypeCommon(op, f3, rs1, rs2) |
            (static_cast<Word>(imm) & 0x1Fu)  << 7 |
            (static_cast<Word>(imm) & 0xFE0)  << 20; // 25 - 5
}

constexpr Word EncodeSTypeInstruction(Opcode op, Function func, int rs1, int rs2, Word imm) noexcept {
    return EncodeSTypeInstruction(op, static_cast<int>(func), rs1, rs2, imm);
}

constexpr Word EncodeBTypeInstruction(Opcode op, int f3, int rs1, int rs2, Word imm) noexcept {
    return  detail::EncodeSBTypeCommon(op, f3, rs1, rs2) |
            (static_cast<Word>(imm) & (1u << 11))   >> 4 |
            (static_cast<Word>(imm) & (0x0Fu << 1)) << 7 |
            (static_cast<Word>(imm) & (1u << 12))   << 19 |
            (static_cast<Word>(imm) & (0x3Fu << 5)) << 20u;
}

constexpr Word EncodeBTypeInstruction(Opcode op, Function func, int rs1, int rs2, Word imm) noexcept {
    return EncodeBTypeInstruction(op, static_cast<int>(func), rs1, rs2, imm);
}

constexpr Word EncodeUTypeInstruction(Opcode op, int rd, Word imm) noexcept {
    return  detail::EncodeUJTypeCommon(op, rd) |
            (static_cast<Word>(imm) & ~0xFFFu);
}

constexpr Word EncodeJTypeInstruction(Opcode op, int rd, Word imm) noexcept {
    return  detail::EncodeUJTypeCommon(op, rd) |
            (static_cast<Word>(imm) & (0xFF << 12)) |
            (static_cast<Word>(imm) & (1u << 11)) << 9 |
            (static_cast<Word>(imm) & (0x3FFu << 1)) << 20 |
            (static_cast<Word>(imm) & (1u << 20)) << 11;
}

} // namespace cpu
} // namespace riscv
