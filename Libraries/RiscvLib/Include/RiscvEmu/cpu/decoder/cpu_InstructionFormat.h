#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/decoder/cpu_Opcodes.h>
#include <RiscvEmu/util/util_Bitfields.h>

namespace riscv {
namespace cpu {

class Instruction {
public:
    constexpr explicit Instruction(Word val) noexcept : m_Val(val) {}

    constexpr auto Get() const noexcept {
        return m_Val;
    }

    constexpr Opcode opcode() const noexcept {
        return static_cast<Opcode>(m_Val & c_OpcodeMask);
    }
private:
    static constexpr Word c_OpcodeMask = 0x3F;
protected:
    Word m_Val = 0;
};

namespace detail {

class SBTypeBase : public Instruction {
    using Instruction::Instruction;

    constexpr auto funct3() const noexcept { return util::ExtractBitfield(m_Val, 12, 3); }
    constexpr auto rs1() const noexcept { return util::ExtractBitfield(m_Val, 15, 5); }
    constexpr auto rs2() const noexcept { return util::ExtractBitfield(m_Val, 20, 5); }
};

class UJTypeBase : public Instruction {
    using Instruction::Instruction;

    constexpr auto rd() const noexcept { return util::ExtractBitfield(m_Val, 7, 5); }
};

} // namespace detail

class RTypeInstruction : public Instruction {
public:
    using Instruction::Instruction;

    constexpr auto rd() const noexcept { return util::ExtractBitfield(m_Val, 7, 5); }
    constexpr auto funct3() const noexcept { return util::ExtractBitfield(m_Val, 12, 3); }
    constexpr auto rs1() const noexcept { return util::ExtractBitfield(m_Val, 15, 5); }
    constexpr auto rs2() const noexcept { return util::ExtractBitfield(m_Val, 20, 5); }
    constexpr auto funct7() const noexcept { return util::ExtractBitfield(m_Val, 25, 7); }
};

class ITypeInstruction : public Instruction {
public:
    using Instruction::Instruction;

    constexpr auto rd() const noexcept { return util::ExtractBitfield(m_Val, 7, 5 ); }
    constexpr auto funct3() const noexcept { return util::ExtractBitfield(m_Val, 12, 3); }
    constexpr auto rs1() const noexcept { return util::ExtractBitfield(m_Val, 15, 5); }
    constexpr auto imm() const noexcept { return util::ExtractBitfield(m_Val, 20, 12); }
};

class STypeInstruction : public detail::SBTypeBase {
public:
    using detail::SBTypeBase::SBTypeBase;

    constexpr auto imm() const noexcept {
        return util::ExtractBitfield(m_Val, 7, 5) | (m_Val & (0x7F << 25) >> 20);
    }
};

class BTypeInstruction : public detail::SBTypeBase {
    using detail::SBTypeBase::SBTypeBase;

    constexpr auto imm() const noexcept {
        return (m_Val & (0xF << 8) >> 7) | (m_Val & (0x3F << 25) >> 20) | (m_Val & (1 << 7) << 4) | (m_Val & (1 << 31) >> 19);
    }
};

class UTypeInstruction : public detail::UJTypeBase {
    using detail::UJTypeBase::UJTypeBase;

    constexpr auto imm() const noexcept { return m_Val & (0xFFF << 12); }
};

class JTypeInstruction : public detail::UJTypeBase {
    using detail::UJTypeBase::UJTypeBase;

    constexpr auto imm() const noexcept {
        return (m_Val & (0x3FF << 21) >> 20) | (m_Val & (1 << 20) >> 9) | (m_Val & (0xFF << 12)) | (m_Val & (1 << 31) >> 11);
    }
}

} // namespace cpu
} // namespace riscv
