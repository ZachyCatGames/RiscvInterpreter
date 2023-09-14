#pragma once
#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>
#include <RiscvEmu/cpu/decoder/cpu_Values.h>

namespace riscv {
namespace cpu {

template<typename Derived>
class DecoderImpl {
public:
    constexpr Result ParseInstruction(Instruction inst) {
        /* Parse starting from opcode. */
        return this->ParseOpcode(inst);
    }
private:
    constexpr auto GetDerived() noexcept { return static_cast<Derived*>(this); }

    /* Derived must provide *Impl functions. */
    constexpr auto CreateInReg(auto val) noexcept { return GetDerived()->CreateInRegImpl(val); }
    constexpr auto CreateOutReg(auto val) noexcept { return GetDerived()->CreateOutRegImpl(val); }
    constexpr auto CreateImmediate(auto val) noexcept { return GetDerived()->CreateImmediateImpl(val); }

    constexpr Result CallStandardRType(RTypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateInReg(inst.rs2()));
    }

    constexpr Result CallStandardITypeExt(ITypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(inst.imm_ext()));
    }

    constexpr Result CallStandardSTypeExt(STypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateInReg(inst.rs1()), CreateInReg(inst.rs2()), CreateImmediate(inst.imm_ext()));
    }

    constexpr Result CallStandardBTypeExt(BTypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateInReg(inst.rs1()), CreateInReg(inst.rs2()), CreateImmediate(inst.imm_ext()));
    }

    constexpr Result CallStandardUTypeExt(UTypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateImmediate(inst.imm_ext()));
    }

    constexpr Result CallStandardJTypeExt(JTypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateImmediate(inst.imm_ext()));
    }

private:
    constexpr Result ParseOpcode(Instruction inst) {
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
            return this->CallStandardUTypeExt(UTypeInstruction(inst), &Derived::ParseInstAUIPC);
        case Opcode::OP_IMM_32:
            return this->ParseOP_IMM_32(ITypeInstruction(inst));
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
            return this->CallStandardUTypeExt(UTypeInstruction(inst), &Derived::ParseInstLUI);
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
            return this->CallStandardJTypeExt(JTypeInstruction(inst), &Derived::ParseInstJAL);
        case Opcode::SYSTEM:
            /* TODO */
            break;
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseLOAD(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::LB:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLB);
        case Funct3::LH:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLH);
        case Funct3::LW:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLW);
        case Funct3::LBU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLBU);
        case Funct3::LHU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLHU);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseMISC_MEM(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::FENCE:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstFENCE);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP_IMM(ITypeInstruction inst) {
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
            auto shamt = imm & ShiftAmtMask;

            /* Check if any upper bits in the immediate are set. */
            auto upper = imm & ~ShiftAmtMask;
            if(upper) {
                /* If exclusively the 10th bit is set, parse as SRAI. */
                if(upper == (1 << 10)) {
                    return GetDerived()->ParseInstSRAI(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(shamt));
                }

                /* If any other bits are set, this is an invalid/reserved instruction. */
                break;
            }

            /* Otherwise parse as SRLI. */
            return GetDerived()->ParseInstSRLI(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(shamt));
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

    constexpr Result ParseOP_IMM_32(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::ADDIW:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstADDIW);
        case Funct3::SLLIW:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLLIW);
        case Funct3::SRLIW: {
            auto imm = inst.imm();
            auto shamt = imm & ShiftAmtMaskFor32;

            /* Check if any upper bits in the immediate are set. */
            auto upper = imm & ~ShiftAmtMaskFor32;
            if(upper) {
                /* If exclusively the 10th bit is set, parse as SRAI. */
                if(upper == (1 << 10)) {
                    return GetDerived()->ParseInstSRAIW(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(shamt));
                }

                /* If any other bits are set, this is an invalid/reserved instruction. */
                break;
            }

            /* Otherwise parse as SRLI. */
            return GetDerived()->ParseInstSRLIW(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(shamt));
        }
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseSTORE(STypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::SB:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSB);
        case Funct3::SH:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSH);
        case Funct3::SW:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSW);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP(RTypeInstruction inst) {
        auto CallOnFunct7Zero = [&](RTypeInstruction inst, auto func) constexpr -> Result {
            if(inst.funct7() == Funct7::Zero)
                return this->CallStandardRType(inst, func);
            return ResultInvalidInstruction();
        };

        auto func7 = inst.funct7();
        switch(inst.funct3()) {
        case Funct3::ADD: {
            switch(func7) {
            case Funct7::ADD:
                return this->CallStandardRType(inst, &Derived::ParseInstADD);
            case Funct7::SUB:
                return this->CallStandardRType(inst, &Derived::ParseInstSUB);
            default:
                break;
            }
            break;
        }
        case Funct3::SLL:
            return CallOnFunct7Zero(inst, &Derived::ParseInstSLL);
        case Funct3::SLT:
            return CallOnFunct7Zero(inst, &Derived::ParseInstSLT);
        case Funct3::SLTU:
            return CallOnFunct7Zero(inst, &Derived::ParseInstSLTU);
        case Funct3::XOR:
            return CallOnFunct7Zero(inst, &Derived::ParseInstXOR);
        case Funct3::SRL: {
            switch(func7) {
            case Funct7::SRL:
                return this->CallStandardRType(inst, &Derived::ParseInstSRL);
            case Funct7::SRA:
                return this->CallStandardRType(inst, &Derived::ParseInstSRA);
            default:
                break;
            }
            break;
        }
        case Funct3::OR:
            return CallOnFunct7Zero(inst, &Derived::ParseInstOR);
        case Funct3::AND:
            return CallOnFunct7Zero(inst, &Derived::ParseInstAND);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseBRANCH(BTypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::BEQ:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBEQ);
        case Funct3::BNE:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBNE);
        case Funct3::BLT:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBLT);
        case Funct3::BGE:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBGE);
        case Funct3::BLTU:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBLTU);
        case Funct3::BGEU:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBGEU);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseJALR(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::JALR:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstJALR);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

}; // class DecoderImpl

} // namespace cpu
} // namespace riscv
