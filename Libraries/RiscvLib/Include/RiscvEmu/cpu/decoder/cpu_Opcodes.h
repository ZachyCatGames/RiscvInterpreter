#pragma once

namespace riscv {
namespace cpu {

enum class Opcode {
    /* Official in-use opcodes. */
    LOAD = 0b0000011,
    LOAD_FP = 0b0000111,
    MISC_MEM = 0b0001111,
    OP_IMM = 0b0010011,
    AUIPC = 0b0010111,
    OP_IMM_32 = 0b0011011,
    STORE = 0b0100011,
    STORE_FP = 0b0100111,
    AMO = 0b0101111,
    OP = 0b0110011,
    LUI = 0b0110111,
    OP_32 = 0b0111011,
    MADD = 0b1000011,
    MSUB = 0b1000111,
    NMSUB = 0b1001011,
    NMADD = 0b1001111,
    OP_FP = 0b1010011,
    BRANCH = 0b1100011,
    JALR = 0b1100111,
    JAL = 0b1101111,
    SYSTEM = 0b1110011,

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
};

} // namespace cpu
} // namespace riscv
