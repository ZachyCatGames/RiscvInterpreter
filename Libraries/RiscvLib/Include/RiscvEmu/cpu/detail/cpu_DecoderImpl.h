#pragma once
#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/cpu_InstructionFormat.h>
#include <RiscvEmu/cpu/cpu_Values.h>

namespace riscv {
namespace cpu {
namespace detail {

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

    constexpr Result CallStandardIType(ITypeInstruction inst, auto func) {
        return (*GetDerived().*func)(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(inst.imm()));
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
            return this->ParseOP_32(RTypeInstruction(inst));
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
            return this->ParseSYSTEM(ITypeInstruction(inst));
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
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Funct3::LD:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLD);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        case Funct3::LBU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLBU);
        case Funct3::LHU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLHU);
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Funct3::LWU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLWU);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseMISC_MEM(ITypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::FENCE:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstFENCE);
        case Funct3::FENCEI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstFENCEI);
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
#ifdef RISCV_CFG_CPU_ENABLE_RV64
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
#endif // RISCV_CFG_CPU_ENABLE_RV64
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
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Funct3::SD:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSD);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP(RTypeInstruction inst) {
        switch(inst.funct3()) {
        case Funct3::ADD: {
            switch(inst.funct7()) {
            case Funct7::ADD:
                return this->CallStandardRType(inst, &Derived::ParseInstADD);
            case Funct7::SUB:
                return this->CallStandardRType(inst, &Derived::ParseInstSUB);
            case Funct7::MUL:
                return this->CallStandardRType(inst, &Derived::ParseInstMUL);
            default:
                break;
            }
            break;
        }
        case Funct3::SLL: {
            switch(inst.funct7()) {
            case Funct7::SLL:
                return this->CallStandardRType(inst, &Derived::ParseInstSLL);
            case Funct7::MULH:
                return this->CallStandardRType(inst, &Derived::ParseInstMULH);
            default:
                break;
            }
            break;
        }
        case Funct3::SLT: {
            switch(inst.funct7()) {
            case Funct7::SLT:
                return this->CallStandardRType(inst, &Derived::ParseInstSLT);
            case Funct7::MULHSU:
                return this->CallStandardRType(inst, &Derived::ParseInstMULHSU);
            default:
                break;
            }
            break;
        }
        case Funct3::SLTU: {
            switch(inst.funct7()) {
            case Funct7::SLTU:
                return this->CallStandardRType(inst, &Derived::ParseInstSLTU);
            case Funct7::MULHU:
                return this->CallStandardRType(inst, &Derived::ParseInstMULHU);
            default:
                break;
            }
            break;
        }
        case Funct3::XOR: {
            switch(inst.funct7()) {
            case Funct7::XOR:
                return this->CallStandardRType(inst, &Derived::ParseInstXOR);
            case Funct7::DIV:
                return this->CallStandardRType(inst, &Derived::ParseInstDIV);
            default:
                break;
            }
            break;
        }
        case Funct3::SRL: {
            switch(inst.funct7()) {
            case Funct7::SRL:
                return this->CallStandardRType(inst, &Derived::ParseInstSRL);
            case Funct7::SRA:
                return this->CallStandardRType(inst, &Derived::ParseInstSRA);
            case Funct7::DIVU:
                return this->CallStandardRType(inst, &Derived::ParseInstDIVU);
            default:
                break;
            }
            break;
        }
        case Funct3::OR: {
            switch(inst.funct7()) {
            case Funct7::OR:
                return this->CallStandardRType(inst, &Derived::ParseInstOR);
            case Funct7::REM:
                return this->CallStandardRType(inst, &Derived::ParseInstREM);
            default:
                break;
            }
            break;
        }
        case Funct3::AND: {
            switch(inst.funct7()) {
            case Funct7::AND:
                return this->CallStandardRType(inst, &Derived::ParseInstAND);
            case Funct7::REMU:
                return this->CallStandardRType(inst, &Derived::ParseInstREMU);
            default:
                break;
            }
            break;
        }
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP_32(RTypeInstruction inst) {
        switch(inst.funct3()) {
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Funct3::ADDW: {
            switch(inst.funct7()) {
            case Funct7::ADDW:
                return this->CallStandardRType(inst, &Derived::ParseInstADDW);
            case Funct7::SUBW:
                return this->CallStandardRType(inst, &Derived::ParseInstSUBW);
            case Funct7::MULW:
                return this->CallStandardRType(inst, &Derived::ParseInstMULW);
            default:
                break;
            }
            break;
        }
        case Funct3::SLLW:
            return this->CallStandardRType(inst, &Derived::ParseInstSLLW);
        case Funct3::DIVW:
            return this->CallStandardRType(inst, &Derived::ParseInstDIVW);
        case Funct3::SRLW: {
            switch(inst.funct7()) {
            case Funct7::SRLW:
                return this->CallStandardRType(inst, &Derived::ParseInstSRLW);
            case Funct7::SRAW:
                return this->CallStandardRType(inst, &Derived::ParseInstSRAW);
            case Funct7::DIVUW:
                return this->CallStandardRType(inst, &Derived::ParseInstDIVUW);
            default:
                break;
            }
            break;
        }
        case Funct3::REMW:
            return this->CallStandardRType(inst, &Derived::ParseInstREMW);
        case Funct3::REMUW:
            return this->CallStandardRType(inst, &Derived::ParseInstREMUW);
#endif // RISCV_CFG_CPU_ENABLE_RV64
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

    constexpr Result ParseSYSTEM(ITypeInstruction inst) {
        auto callCsrImm = [this](ITypeInstruction inst, auto func) {
            return (*this->GetDerived().*func)(CreateOutReg(inst.rd()), CreateImmediate(static_cast<Word>(inst.rs1())), CreateImmediate(inst.imm()));
        };

        switch(inst.funct3()) {
        case Funct3::CSRRW:
            return this->CallStandardIType(inst, &Derived::ParseInstCSRRW);
        case Funct3::CSRRS:
            return this->CallStandardIType(inst, &Derived::ParseInstCSRRS);
        case Funct3::CSRRC:
            return this->CallStandardIType(inst, &Derived::ParseInstCSRRC);
        case Funct3::CSRRWI:
            return callCsrImm(inst, &Derived::ParseInstCSRRWI);
        case Funct3::CSRRSI:
            return callCsrImm(inst, &Derived::ParseInstCSRRSI);
        case Funct3::CSRRCI:
            return callCsrImm(inst, &Derived::ParseInstCSRRCI);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

}; // class DecoderImpl

} // namespace detail
} // namespace cpu
} // namespace riscv
