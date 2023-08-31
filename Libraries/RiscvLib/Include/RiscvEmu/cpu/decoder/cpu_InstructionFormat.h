#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/decoder/cpu_Opcodes.h>
#include <RiscvEmu/util/util_Bitfields.h>
#include <RiscvEmu/util/util_SignExtend.h>

namespace riscv {
namespace cpu {

namespace detail {

class InstructionBase {
public:
    constexpr explicit InstructionBase(Word val) noexcept : m_Val(val) {}

    constexpr auto Get() const noexcept { return m_Val; }
private:
    Word m_Val = 0;
}; // class InstructionBase

} // namespace detail

class Instruction : public detail::InstructionBase {
public:
    constexpr explicit Instruction(const detail::InstructionBase inst) noexcept : detail::InstructionBase(inst) {}
    constexpr explicit Instruction(Word val) noexcept : detail::InstructionBase(val) {}

    constexpr auto opcode() const noexcept {
        return static_cast<Opcode>(this->Get() & c_OpcodeMask);
    }
private:
    static constexpr Word c_OpcodeMask = 0x3F;
};

namespace detail {

class SBTypeBase : public Instruction {
public:
    using Instruction::Instruction;

    constexpr auto funct3() const noexcept { return static_cast<Funct3>(util::ExtractBitfield(this->Get(), 12, 3)); }
    constexpr auto rs1() const noexcept { return util::ExtractBitfield(this->Get(), 15, 5); }
    constexpr auto rs2() const noexcept { return util::ExtractBitfield(this->Get(), 20, 5); }
};

class UJTypeBase : public Instruction {
public:
    using Instruction::Instruction;

    constexpr auto rd() const noexcept { return util::ExtractBitfield(this->Get(), 7, 5); }
};

} // namespace detail

class RTypeInstruction : public Instruction {
public:
    using Instruction::Instruction;

    constexpr auto rd() const noexcept { return util::ExtractBitfield(this->Get(), 7, 5); }
    constexpr auto funct3() const noexcept { return static_cast<Funct3>(util::ExtractBitfield(this->Get(), 12, 3)); }
    constexpr auto rs1() const noexcept { return util::ExtractBitfield(this->Get(), 15, 5); }
    constexpr auto rs2() const noexcept { return util::ExtractBitfield(this->Get(), 20, 5); }
    constexpr auto funct7() const noexcept { return static_cast<Funct7>(util::ExtractBitfield(this->Get(), 25, 7)); }
};

class ITypeInstruction : public Instruction {
public:
    using Instruction::Instruction;

    constexpr auto rd() const noexcept { return util::ExtractBitfield(this->Get(), 7, 5 ); }
    constexpr auto funct3() const noexcept { return static_cast<Funct3>(util::ExtractBitfield(this->Get(), 12, 3)); }
    constexpr auto rs1() const noexcept { return util::ExtractBitfield(this->Get(), 15, 5); }
    constexpr auto imm() const noexcept { return util::ExtractBitfield(this->Get(), 20, 12); }
    constexpr auto imm_ext() const noexcept { return util::SignExtend(static_cast<NativeWord>(this->imm()), 12, NativeWordBitLen); }
};

class STypeInstruction : public detail::SBTypeBase {
public:
    using detail::SBTypeBase::SBTypeBase;

    constexpr auto imm() const noexcept {
        return util::ExtractBitfield(this->Get(), 7u, 5u) | (this->Get() & (0x7Fu << 25u) >> 20u);
    }

    constexpr auto imm_ext() const noexcept { return util::SignExtend(static_cast<NativeWord>(this->imm()), 12, NativeWordBitLen); }
};

class BTypeInstruction : public detail::SBTypeBase {
public:
    using detail::SBTypeBase::SBTypeBase;

    constexpr auto imm() const noexcept {
        return (this->Get() & (0xFu << 8u) >> 7u) | (this->Get() & (0x3Fu << 25u) >> 20u) | (this->Get() & (1u << 7u) << 4u) | (this->Get() & (1u << 31u) >> 19u);
    }

    constexpr auto imm_ext() const noexcept { return util::SignExtend(static_cast<NativeWord>(this->imm()), 13u, NativeWordBitLen); }
};

class UTypeInstruction : public detail::UJTypeBase {
public:
    using detail::UJTypeBase::UJTypeBase;

    constexpr auto imm() const noexcept { return this->Get() & (0xFFFFFFFFu << 12u); }

    constexpr auto imm_ext() const noexcept { return util::SignExtend(static_cast<NativeWord>(this->imm()), 32, NativeWordBitLen); }
};

class JTypeInstruction : public detail::UJTypeBase {
public:
    using detail::UJTypeBase::UJTypeBase;

    constexpr auto imm() const noexcept {
        return (this->Get() & (0x3FFu << 21u) >> 20u) | (this->Get() & (1u << 20u) >> 9u) | (this->Get() & (0xFFu << 12u)) | (this->Get() & (1u << 31u) >> 11u);
    }

    constexpr auto imm_ext() const noexcept { return util::SignExtend(static_cast<NativeWord>(this->imm()), 21, NativeWordBitLen); }
};

} // namespace cpu
} // namespace riscv
