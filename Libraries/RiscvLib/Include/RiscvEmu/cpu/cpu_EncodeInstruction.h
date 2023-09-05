#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/decoder/cpu_Opcodes.h>

namespace riscv {
namespace cpu {

constexpr Word EncodeRTypeInstruction(Opcode op, Funct3 f3, Funct7 f7, int rd, int rs1, int rs2) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(rd)  & 0x3Fu) << 7 |
            (static_cast<Word>(f3)  & 0x07u) << 12 |
            (static_cast<Word>(rs1) & 0x3Fu) << 15 |
            (static_cast<Word>(rs2) & 0x3Fu) << 20 |
            (static_cast<Word>(f7)  & 0x7Fu) << 25;
}

constexpr Word EncodeITypeInstruction(Opcode op, Funct3 f3, int rd, int rs1, Word imm) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(rd)  & 0x3Fu) << 7 |
            (static_cast<Word>(f3)  & 0x07u) << 12 |
            (static_cast<Word>(rs1) & 0x3Fu) << 15 |
            (static_cast<Word>(imm))         << 20;
}

constexpr Word EncodeSTypeInstruction(Opcode op, Funct3 f3, int rs1, int rs2, Word imm) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(imm) & 0x0Fu)  << 7 |
            (static_cast<Word>(f3)  & 0x07u)  << 12 |
            (static_cast<Word>(rs1) & 0x3Fu)  << 15 |
            (static_cast<Word>(rs2) & 0x3Fu)  << 20 |
            (static_cast<Word>(imm) & 0xFF0u) << 25;
}

constexpr Word EncodeBTypeInstruction(Opcode op, Funct3 f3, int rs1, int rs2, Word imm) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(imm) & 1u << 11)   >> 4 |
            (static_cast<Word>(imm) & 0x0Fu << 1) << 8 |
            (static_cast<Word>(f3)  & 0x07u)      << 12 |
            (static_cast<Word>(rs1) & 0x3Fu)      << 15 |
            (static_cast<Word>(rs2) & 0x3Fu)      << 20 |
            (static_cast<Word>(imm) & 1u << 12)   << 19 |
            (static_cast<Word>(imm) & 0x06u << 5) << 20u;
}

constexpr Word EncodeUTypeInstruction(Opcode op, int rd, Word imm) noexcept {
    return  (static_cast<Word>(op)  & 0x7Fu) |
            (static_cast<Word>(rd)  & 0x3Fu) << 7 |
            (static_cast<Word>(imm) & ~0xFFF);
}

} // namespace cpu
} // namespace riscv
