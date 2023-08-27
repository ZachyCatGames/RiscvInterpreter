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

    constexpr Result CallStandardUTypeExt(UTypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateImmediate(inst.imm_ext()));
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

    Result ParseMISC_MEM(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case 0b000: // FENCE
            break;
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    Result ParseOP_IMM(ITypeInstruction inst) {
        static constexpr auto shamtMask = cfg::cpu::EnableIsaRV64I ? 0x3F : 0x1F;

        switch(inst.funct3()) {
        case Funct3::ADDI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstADDI);
        case Funct3::SLLI: {
            /* SLLI requires that all upper bits are 0. */
            if(inst.imm() > NativeWordBitLen - 1) {
                break;
            }
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLLI);
        }
        case Funct3::SLTI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLTI);
        case Funct3::SLTIU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLTIU);
        case Funct3::XORI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstXORI);
        case Funct3::SRLI: {
            auto imm = inst.imm();
            auto shamt = imm & shamtMask;

            /* Check if any upper bits in the immediate are set. */
            auto upper = imm & ~shamtMask;
            if(upper) {
                /* If exclusively the 10th bit is set, parse as SRAI. */
                if(upper == (1 << 10)) {
                    return GetDerived().ParseInstSRAI(CreateOutReg(inst.rd()), CreateInReg(inst.rs1), CreateImmediate(shamt));
                }

                /* If any other bits are set, this is an invalid/reserved instruction. */
                break;
            }

            /* Otherwise parse as SRLI. */
            return GetDerived().ParseInstSRLI(CreateOutReg(inst.rd()), CreateInReg(inst.rs1), CreateImmediate(shamt));
        }
        case Funct3::ORI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstORI);
        case Funct3::ANDI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstANDI);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

}; // class DecoderImpl

} // namespace cpu
} // namespace riscv
