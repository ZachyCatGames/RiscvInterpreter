#pragma once

namespace riscv {
namespace cpu {

enum class Opcode {
    /* Lowest possible opcode. */
    Min = 0b0000000,

    /* Official in-use opcodes. */
    LOAD      = 0b0000011,
    LOAD_FP   = 0b0000111,
    MISC_MEM  = 0b0001111,
    OP_IMM    = 0b0010011,
    AUIPC     = 0b0010111,
    OP_IMM_32 = 0b0011011,
    STORE     = 0b0100011,
    STORE_FP  = 0b0100111,
    AMO       = 0b0101111,
    OP        = 0b0110011,
    LUI       = 0b0110111,
    OP_32     = 0b0111011,
    MADD      = 0b1000011,
    MSUB      = 0b1000111,
    NMSUB     = 0b1001011,
    NMADD     = 0b1001111,
    OP_FP     = 0b1010011,
    BRANCH    = 0b1100011,
    JALR      = 0b1100111,
    JAL       = 0b1101111,
    SYSTEM    = 0b1110011,

    /* Reserved for custom extensions. */
    CUSTOM_0 = 0b0001011,
    CUSTOM_1 = 0b0101011,

    /* Reserved for future use on RV128, reserved for custom use on non-RV128. */
    CUSTOM_RV128_RESERVED_1 = 0b1011011,
    CUSTOM_RV128_RESERVED_2 = 0b1111011,

    /* Reserved for future use. */
    RESERVED_0 = 0b1010111,
    RESERVED_1 = 0b1101011,
    RESERVED_3 = 0b1110111,

    /* Reserved for higher length instructions. */
    RESERVED_48_1 = 0b0011111,
    RESERVED_64   = 0b0111111,
    RESERVED_48_2 = 0b1011111,
    RESERVED_80   = 0b1111111,

    /* Highest possible opcode. */
    Max = 0b1111111
}; // enum class Opcode

enum class Funct3 {
    /* Lowest possible Funct3 value. */
    Min = 0b000,

    /* Opcode LOAD. */
    LB  = 0b000,
    LH  = 0b001,
    LW  = 0b010,
    LD  = 0b011,
    LBU = 0b100,
    LHU = 0b101,
    LWU = 0b110,

    /* Opcode MISC_MEM. */
    FENCE  = 0b000,
    FENCEI = 0b001,

    /* Opcode OP_IMM. */
    ADDI  = 0b000,
    SLLI  = 0b001,
    SLTI  = 0b010,
    SLTIU = 0b011,
    XORI  = 0b100,
    SRLI  = 0b101,
    ORI   = 0b110,
    ANDI  = 0b111,

    /* Opcode OP_IMM_32. */
    ADDIW = 0b000,
    SLLIW = 0b001,
    SRLIW = 0b101,

    /* Opcode STORE. */
    SB = 0b000,
    SH = 0b001,
    SW = 0b010,
    SD = 0b011,

    /* Opcode OP. */
    ADD    = 0b000,
    SLL    = 0b001,
    SLT    = 0b010,
    SLTU   = 0b011,
    XOR    = 0b100,
    SRL    = 0b101,
    OR     = 0b110,
    AND    = 0b111,
    MUL    = ADD, // M extension.
    MULH   = SLL,
    MULHSU = SLT,
    MULHU  = SLTU,
    DIV    = XOR,
    DIVU   = SRL,
    REM    = OR,
    REMU   = AND,

    /* Opcode OP_32. */
    ADDW  = 0b000,
    SLLW  = 0b001,
    SRLW  = 0b101,
    MULW  = ADDW, // M extension.
    DIVW  = 0b100,
    DIVUW = SRLW,
    REMW  = 0b110,
    REMUW = 0b111,

    /* Opcode BRANCH. */
    BEQ  = 0b000,
    BNE  = 0b001,
    BLT  = 0b100,
    BGE  = 0b101,
    BLTU = 0b110,
    BGEU = 0b111,

    /* Opcode JALR. */
    JALR = 0b000,

    /* Opcode SYSTEM. */
    CSRRW = 0b001,
    CSRRS = 0b010,
    CSRRC = 0b011,
    CSRRWI = 0b101,
    CSRRSI = 0b110,
    CSRRCI = 0b111,

    /* Highest possible Funct3 value. */
    Max = 0b111
}; // enum class Funct3

enum class Funct7 {
    /* Lowest possible Funct7 value. */
    Min = 0b0000000,

    /* For use when Funct7 isn't used within a Funct3 namespace. */
    Zero = 0,

    /* OP::ADD. */
    ADD = 0b0000000,
    SUB = 0b0100000,
    MUL = 0b0000001,

    /* OP::SLL. */
    SLL  = 0b0000000,
    MULH = 0b0000001,

    /* OP::SLT. */
    SLT    = 0b0000000,
    MULHSU = 0b0000001,

    /* OP::SLTU. */
    SLTU  = 0b0000000,
    MULHU = 0b0000001,

    /* OP::XOR. */
    XOR = 0b0000000,
    DIV = 0b0000001,

    /* OP::SRL. */
    SRL  = 0b0000000,
    SRA  = 0b0100000,
    DIVU = 0b0000001,

    /* OP::OR. */
    OR  = 0b0000000,
    REM = 0b0000001,

    /* OP::AND. */
    AND  = 0b0000000,
    REMU = 0b0000001,

    /* OP_32::ADDW. */
    ADDW = 0b0000000,
    SUBW = 0b0100000,
    MULW = 0b0000001,

    /* OP_32::SRLW. */
    SRLW = 0b0000000,
    SRAW = 0b0100000,
    DIVUW = 0b0000001,

    /* Highest possible Funct7 value. */
    Max = 0b1111111
};

} // namespace cpu
} // namespace riscv
