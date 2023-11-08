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

namespace detail {
    constexpr int CreateFunctionImpl3(int f3) noexcept { return f3; }

    constexpr int CreateFunctionImpl37(int f3, int f7) noexcept { return f3 | (f7 << 3); }
} // namespace detail

enum class Function {
    Min = 0,
    Max = detail::CreateFunctionImpl37(0b111, 0b1111111),

    /* Opcode LOAD. */
    LB  = detail::CreateFunctionImpl3(0b000),
    LH  = detail::CreateFunctionImpl3(0b001),
    LW  = detail::CreateFunctionImpl3(0b010),
    LD  = detail::CreateFunctionImpl3(0b011),
    LBU = detail::CreateFunctionImpl3(0b100),
    LHU = detail::CreateFunctionImpl3(0b101),
    LWU = detail::CreateFunctionImpl3(0b110),

    /* Opcode MISC_MEM. */
    FENCE  = detail::CreateFunctionImpl3(0b000),
    FENCEI = detail::CreateFunctionImpl3(0b001),

    /* Opcode OP_IMM. */
    ADDI  = detail::CreateFunctionImpl3(0b000),
    SLLI  = detail::CreateFunctionImpl3(0b001),
    SLTI  = detail::CreateFunctionImpl3(0b010),
    SLTIU = detail::CreateFunctionImpl3(0b011),
    XORI  = detail::CreateFunctionImpl3(0b100),
    SRLI  = detail::CreateFunctionImpl3(0b101),
    ORI   = detail::CreateFunctionImpl3(0b110),
    ANDI  = detail::CreateFunctionImpl3(0b111),

    /* Opcode OP_IMM_32. */
    ADDIW = detail::CreateFunctionImpl3(0b000),
    SLLIW = detail::CreateFunctionImpl3(0b001),
    SRLIW = detail::CreateFunctionImpl3(0b101),

    /* Opcode STORE. */
    SB = detail::CreateFunctionImpl3(0b000),
    SH = detail::CreateFunctionImpl3(0b001),
    SW = detail::CreateFunctionImpl3(0b010),
    SD = detail::CreateFunctionImpl3(0b011),

    /* Opcode OP. */
    ADD    = detail::CreateFunctionImpl37(0b000, 0b0000000),
    SUB    = detail::CreateFunctionImpl37(0b000, 0b0100000),
    MUL    = detail::CreateFunctionImpl37(0b000, 0b0000001),
    SLL    = detail::CreateFunctionImpl37(0b001, 0b0000000),
    MULH   = detail::CreateFunctionImpl37(0b001, 0b0000001),
    SLT    = detail::CreateFunctionImpl37(0b010, 0b0000000),
    MULHSU = detail::CreateFunctionImpl37(0b010, 0b0000001),
    SLTU   = detail::CreateFunctionImpl37(0b011, 0b0000000),
    MULHU  = detail::CreateFunctionImpl37(0b011, 0b0000001),
    XOR    = detail::CreateFunctionImpl37(0b100, 0b0000000),
    DIV    = detail::CreateFunctionImpl37(0b100, 0b0000001),
    SRL    = detail::CreateFunctionImpl37(0b101, 0b0000000),
    SRA    = detail::CreateFunctionImpl37(0b101, 0b0100000),
    DIVU   = detail::CreateFunctionImpl37(0b101, 0b0000001),
    OR     = detail::CreateFunctionImpl37(0b110, 0b0000000),
    REM    = detail::CreateFunctionImpl37(0b110, 0b0000001),
    AND    = detail::CreateFunctionImpl37(0b111, 0b0000000),
    REMU   = detail::CreateFunctionImpl37(0b111, 0b0000001),

    /* Opcode OP_32. */
    ADDW  = detail::CreateFunctionImpl37(0b000, 0b0000000),
    SUBW  = detail::CreateFunctionImpl37(0b000, 0b0100000),
    MULW  = detail::CreateFunctionImpl37(0b000, 0b0000001),
    SLLW  = detail::CreateFunctionImpl37(0b001, 0b0000000),
    DIVW  = detail::CreateFunctionImpl37(0b100, 0b0000001),
    SRLW  = detail::CreateFunctionImpl37(0b101, 0b0000000),
    SRAW  = detail::CreateFunctionImpl37(0b101, 0b0100000),
    DIVUW = detail::CreateFunctionImpl37(0b101, 0b0000001),
    REMW  = detail::CreateFunctionImpl37(0b110, 0b0000001),
    REMUW = detail::CreateFunctionImpl37(0b111, 0b0000001),

    /* Opcode BRANCH. */
    BEQ  = detail::CreateFunctionImpl3(0b000),
    BNE  = detail::CreateFunctionImpl3(0b001),
    BLT  = detail::CreateFunctionImpl3(0b100),
    BGE  = detail::CreateFunctionImpl3(0b101),
    BLTU = detail::CreateFunctionImpl3(0b110),
    BGEU = detail::CreateFunctionImpl3(0b111),

    /* Opcode JALR. */
    JALR = detail::CreateFunctionImpl3(0b000),

    /* Opcode SYSTEM. */
    CSRRW = detail::CreateFunctionImpl3(0b001),
    CSRRS = detail::CreateFunctionImpl3(0b010),
    CSRRC = detail::CreateFunctionImpl3(0b011),
    CSRRWI = detail::CreateFunctionImpl3(0b101),
    CSRRSI = detail::CreateFunctionImpl3(0b110),
    CSRRCI = detail::CreateFunctionImpl3(0b111),
};

constexpr Function CreateFunction3(int f3) noexcept { return static_cast<Function>(detail::CreateFunctionImpl3(f3)); }

constexpr Function CreateFunction37(int f3, int f7) noexcept { return static_cast<Function>(detail::CreateFunctionImpl37(f3, f7)); }

constexpr int GetFunction3(int func) noexcept { return func & 0x7; }
constexpr int GetFunction3(Function func) noexcept { return GetFunction3(static_cast<int>(func)); }

constexpr int GetFunction7(int func) noexcept { return func >> 3; }
constexpr int GetFunction7(Function func) noexcept { return GetFunction7(static_cast<int>(func)); }

} // namespace cpu
} // namespace riscv
