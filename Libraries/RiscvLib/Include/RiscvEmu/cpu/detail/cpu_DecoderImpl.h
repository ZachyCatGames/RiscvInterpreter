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
        switch(inst.function()) {
        case Function::LB:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLB);
        case Function::LH:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLH);
        case Function::LW:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLW);
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Function::LD:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLD);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        case Function::LBU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLBU);
        case Function::LHU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLHU);
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Function::LWU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstLWU);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseMISC_MEM(ITypeInstruction inst) {
        switch(inst.function()) {
        case Function::FENCE:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstFENCE);
        case Function::FENCEI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstFENCEI);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP_IMM(ITypeInstruction inst) {
        switch(inst.function()) {
        case Function::ADDI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstADDI);
        case Function::SLLI: {
            /* SLLI requires that all upper bits are 0. */
            if(inst.imm() > NativeWordBitLen - 1) {
                break;
            }
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLLI);
        }
        case Function::SLTI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLTI);
        case Function::SLTIU:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLTIU);
        case Function::XORI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstXORI);
        case Function::SRLI: {
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
        case Function::ORI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstORI);
        case Function::ANDI:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstANDI);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP_IMM_32(ITypeInstruction inst) {
        switch(inst.function()) {
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Function::ADDIW:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstADDIW);
        case Function::SLLIW:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstSLLIW);
        case Function::SRLIW: {
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
        switch(inst.function()) {
        case Function::SB:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSB);
        case Function::SH:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSH);
        case Function::SW:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSW);
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Function::SD:
            return this->CallStandardSTypeExt(inst, &Derived::ParseInstSD);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP(RTypeInstruction inst) {
        switch(inst.function()) {
        case Function::ADD:
            return this->CallStandardRType(inst, &Derived::ParseInstADD);
        case Function::SUB:
            return this->CallStandardRType(inst, &Derived::ParseInstSUB);
        case Function::MUL:
            return this->CallStandardRType(inst, &Derived::ParseInstMUL);
        case Function::SLL:
            return this->CallStandardRType(inst, &Derived::ParseInstSLL);
        case Function::MULH:
            return this->CallStandardRType(inst, &Derived::ParseInstMULH);
        case Function::SLT:
            return this->CallStandardRType(inst, &Derived::ParseInstSLT);
        case Function::MULHSU:
            return this->CallStandardRType(inst, &Derived::ParseInstMULHSU);
        case Function::SLTU:
            return this->CallStandardRType(inst, &Derived::ParseInstSLTU);
        case Function::MULHU:
            return this->CallStandardRType(inst, &Derived::ParseInstMULHU);
        case Function::XOR:
            return this->CallStandardRType(inst, &Derived::ParseInstXOR);
        case Function::DIV:
            return this->CallStandardRType(inst, &Derived::ParseInstDIV);
        case Function::SRL:
            return this->CallStandardRType(inst, &Derived::ParseInstSRL);
        case Function::SRA:
            return this->CallStandardRType(inst, &Derived::ParseInstSRA);
        case Function::DIVU:
            return this->CallStandardRType(inst, &Derived::ParseInstDIVU);
        case Function::OR:
            return this->CallStandardRType(inst, &Derived::ParseInstOR);
        case Function::REM:
            return this->CallStandardRType(inst, &Derived::ParseInstREM);
        case Function::AND:
            return this->CallStandardRType(inst, &Derived::ParseInstAND);
        case Function::REMU:
            return this->CallStandardRType(inst, &Derived::ParseInstREMU);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseOP_32(RTypeInstruction inst) {
        switch(inst.function()) {
#ifdef RISCV_CFG_CPU_ENABLE_RV64
        case Function::ADDW:
            return this->CallStandardRType(inst, &Derived::ParseInstADDW);
        case Function::SUBW:
            return this->CallStandardRType(inst, &Derived::ParseInstSUBW);
        case Function::MULW:
            return this->CallStandardRType(inst, &Derived::ParseInstMULW);
        case Function::SLLW:
            return this->CallStandardRType(inst, &Derived::ParseInstSLLW);
        case Function::DIVW:
            return this->CallStandardRType(inst, &Derived::ParseInstDIVW);
        case Function::SRLW:
            return this->CallStandardRType(inst, &Derived::ParseInstSRLW);
        case Function::SRAW:
            return this->CallStandardRType(inst, &Derived::ParseInstSRAW);
        case Function::DIVUW:
            return this->CallStandardRType(inst, &Derived::ParseInstDIVUW);
        case Function::REMW:
            return this->CallStandardRType(inst, &Derived::ParseInstREMW);
        case Function::REMUW:
            return this->CallStandardRType(inst, &Derived::ParseInstREMUW);
#endif // RISCV_CFG_CPU_ENABLE_RV64
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseBRANCH(BTypeInstruction inst) {
        switch(inst.function()) {
        case Function::BEQ:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBEQ);
        case Function::BNE:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBNE);
        case Function::BLT:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBLT);
        case Function::BGE:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBGE);
        case Function::BLTU:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBLTU);
        case Function::BGEU:
            return this->CallStandardBTypeExt(inst, &Derived::ParseInstBGEU);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseJALR(ITypeInstruction inst) {
        switch(inst.function()) {
        case Function::JALR:
            return this->CallStandardITypeExt(inst, &Derived::ParseInstJALR);
        default:
            break;
        }

        return ResultInvalidInstruction();
    }

    constexpr Result ParseSYSTEM(ITypeInstruction inst) {
        auto callCsrWithRs1Val = [this](ITypeInstruction inst, auto func) {
            return (*this->GetDerived().*func)(CreateOutReg(inst.rd()), CreateInReg(inst.rs1()), CreateImmediate(inst.imm()), CreateImmediate(static_cast<Word>(inst.rs1())));
        };

        auto callCsrImm = [this](ITypeInstruction inst, auto func) {
            return (*this->GetDerived().*func)(CreateOutReg(inst.rd()), CreateImmediate(static_cast<Word>(inst.rs1())), CreateImmediate(inst.imm()));
        };

        switch(inst.function()) {
        case Function::CSRRW:
            return this->CallStandardIType(inst, &Derived::ParseInstCSRRW);
        case Function::CSRRS:
            return callCsrWithRs1Val(inst, &Derived::ParseInstCSRRS);
        case Function::CSRRC:
            return callCsrWithRs1Val(inst, &Derived::ParseInstCSRRC);
        case Function::CSRRWI:
            return callCsrImm(inst, &Derived::ParseInstCSRRWI);
        case Function::CSRRSI:
            return callCsrImm(inst, &Derived::ParseInstCSRRSI);
        case Function::CSRRCI:
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
