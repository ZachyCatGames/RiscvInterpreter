#pragma once
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>

namespace riscv {
namespace cpu {

template<typename Derived>
class DecoderImpl {
public:
    Result ParseInstruction(Instruction inst) {
        /* Parse */
    }
private:
    constexpr auto GetDerived() noexcept { return static_cast<Derived*>(this); }

    using InRegObject = Derived::InRegObject;
    using OutRegObject = Derived::OutRegObject;
    using InOutRegObject = Derived::InOutRegObject;
    using ImmediateObject = Derived::ImmediateObject;

    constexpr auto CreateInReg(auto val) noexcept { return InRegObject(GetDerived(), val); }
    constexpr auto CreateOutReg(auto val) noexcept { return OutRegObject(GetDerived(), val); }
    constexpr auto CreateInOutReg(auto val) noexcept { return InOutRegObject(GetDerived(), val); }
    constexpr auto CreateImmediate(auto val) noexcept { return ImmediateObject(GetDerived(), val); }

    constexpr Result CallStandardITypeExt(ITypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(inst.imm_ext()));
    }

private:
    Result ParseOpcode(Instruction inst) {
        switch(inst.opcode()) {
        case Opcode::LOAD:
            return this->ParseLOAD(ITypeInstruction(inst));
        case Opcode::LOAD_FP:
            /* TODO */
            break;
        case Opcode::MISC_MEM:
            return this->ParseMISC_MEM(ITypeInstruction(inst));
        case Opcode::OP_IMM:
            return this->ParseOP_IMM(ITypeInstruction(inst));
        case Opcode::AUIPC:
            return this->ParseAUIPC(UTypeInstruction(inst));
        case Opcode::OP_IMM_32:
            /* TODO */
            break;
        case Opcode::STORE:
            return this->ParseSTORE(STypeInstruction(inst));
        case Opcode::STORE_FP:
            /* TODO */
            break;
        case Opcode::AMO:
            /* TODO */
            break;
        case Opcode::OP:
            return this->ParseOP(RTypeInstruction(inst));
        case Opcode::LUI:
            return this->ParseLUI(UTypeInstruction(inst));
        case Opcode::OP_32:
            /* TODO */
            break;
        case Opcode::MADD:
            /* TODO */
            break;
        case Opcode::MSUB:
            /* TODO */
            break;
        case Opcode::NMSUB:
            /* TODO */
            break;
        case Opcode::NMADD:
            /* TODO */
            break;
        case Opcode::OP_FP:
            /* TODO */
            break;
        case Opcode::BRANCH:
            return this->ParseBRANCH(BTypeInstruction(inst));
        case Opcode::JALR:
            return this->ParseJALR(ITypeInstruction(inst));
        case Opcode::JAL:
            return this->ParseJAL(JTypeInstruction(inst));
        case Opcode::SYSTEM:
            /* TODO */
            break;
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    Result ParseLOAD(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case 0b000: // LB
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLB);
        case 0b001: // LH
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLH);
        case 0b010: // LW
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLW);
        case 0b100: // LBU
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLBU);
        case 0b101: // LHU
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLHU);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

}; // class DecoderImpl

} // namespace cpu
} // namespace riscv
