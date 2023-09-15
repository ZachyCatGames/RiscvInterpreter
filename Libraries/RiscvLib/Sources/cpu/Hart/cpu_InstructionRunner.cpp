#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/cpu/decoder/cpu_DecoderImpl.h>
#include <RiscvEmu/cpu/decoder/cpu_Values.h>
#include <bit>
#include <concepts>

namespace riscv {
namespace cpu {


class Hart::InstructionRunner : public DecoderImpl<Hart::InstructionRunner> {
public:
    constexpr InstructionRunner(Hart* pParent) :
        m_pParent(pParent) {}

private:
    friend class DecoderImpl<Hart::InstructionRunner>;
    class InRegObject {
    public:
        constexpr InRegObject(NativeWord value) :
            m_RegValue(value) {}

        template<std::integral T>
        constexpr auto Get() const noexcept {
            if constexpr (std::signed_integral<T>) {
                return static_cast<T>(static_cast<NativeWordS>(m_RegValue));
            }
            return static_cast<T>(m_RegValue);
        }
    private:
        NativeWord m_RegValue;
    }; // class InRegObject

    class OutRegObject {
    public:
        constexpr OutRegObject(NativeWord* pTarget) :
            m_pTarget(pTarget) {}

        template<std::integral T>
        constexpr void Set(const T& value) noexcept {
            if constexpr (std::signed_integral<T>) {
                *m_pTarget = static_cast<NativeWord>(static_cast<NativeWordS>(value));
            }
            *m_pTarget = static_cast<NativeWord>(value);
        }
    private:
        NativeWord* const m_pTarget;
    }; // class OutRegObject

    class ImmediateObject {
    public:
        constexpr ImmediateObject(NativeWord value) :
            m_Value(value) {}

        template<std::integral T>
        constexpr auto Get() const noexcept {
            if constexpr (std::signed_integral<T>) {
                return static_cast<T>(static_cast<WordS>(m_Value));
            }
            return static_cast<T>(m_Value);
        }
    private:
        NativeWord m_Value;
    }; // class ImmediateObject

    constexpr auto CreateInRegImpl(auto id) noexcept {
        return InRegObject(m_pParent->m_GPR[id]);
    }

    constexpr auto CreateOutRegImpl(auto id) noexcept {
        return OutRegObject(&m_pParent->m_GPR[id]);
    }

    constexpr auto CreateImmediateImpl(auto val) noexcept {
        return ImmediateObject(val);
    }

    /*
     * Opcode LOAD.
     */
    template<bool Signed, typename T>
    Result InstLoadImpl(auto func, OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        T out = 0;
        Result res = (*m_pParent.*func)(&out, rs1.Get<Address>() + imm.Get<Address>());
        if(res.IsSuccess()) {
            if constexpr(Signed) {
                rd.Set(util::SignExtend(static_cast<NativeWord>(out), sizeof(T) * 8, NativeWordBitLen));
            }
            else {
                rd.Set(out);
            }
        }
        return res;
    }
    Result ParseInstLB(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, Byte>(&Hart::MemoryReadByte, rd, rs1, imm);
    }
    Result ParseInstLH(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, HWord>(&Hart::MemoryReadHWord, rd, rs1, imm);
    }
    Result ParseInstLW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, Word>(&Hart::MemoryReadWord, rd, rs1, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstLD(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, DWord>(&Hart::MemoryReadDWord, rd, rs1, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstLBU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, Byte>(&Hart::MemoryReadByte, rd, rs1, imm);
    }
    Result ParseInstLHU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, HWord>(&Hart::MemoryReadHWord, rd, rs1, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstLWU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, Word>(&Hart::MemoryReadWord, rd, rs1, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode MISC_MEM.
     */
    Result ParseInstFENCE([[maybe_unused]] OutRegObject rd, [[maybe_unused]] InRegObject rs1, [[maybe_unused]] ImmediateObject imm) {
        /* 
         * We're always going to be executing instructions in-order,
         * so FENCE doesn't need to be implemented.
         */
        return ResultSuccess();
    }
    Result ParseInstFENCEI([[maybe_unused]] OutRegObject rd, [[maybe_unused]] InRegObject rs1, [[maybe_unused]] ImmediateObject imm) {
        /*
         * FENCE.I is used to sync instruction fetches and instructions writes.
         * We don't do any funky out of order stuff and don't need to implement this.
         */
        return ResultSuccess();
    }

    /*
     * Opcode OP_IMM.
     */
    Result ParseInstADDI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() + imm.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstSLLI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() << imm.Get<Word>());
        return ResultSuccess();
    }
    Result ParseInstSLTI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWordS>() < imm.Get<NativeWordS>());
        return ResultSuccess();
    }
    Result ParseInstSLTIU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() < imm.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstXORI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() ^ imm.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstSRLI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() >> imm.Get<Word>());
        return ResultSuccess();
    }
    Result ParseInstSRAI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWordS>() >> imm.Get<Word>());
        return ResultSuccess();
    }
    Result ParseInstORI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() | imm.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstANDI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<NativeWord>() & imm.Get<NativeWord>());
        return ResultSuccess();
    }

    /*
     * Opcode AUIPC.
     */
    Result ParseInstAUIPC(OutRegObject rd, ImmediateObject imm) {
        rd.Set(m_pParent->m_PC + imm.Get<Address>());
        return ResultSuccess();
    }

#ifdef RISCV_CFG_CPU_ENABLE_RV64
    /*
     * Opcode OP_IMM_32.
     */
    Result ParseInstADDIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<WordS>() + imm.Get<WordS>());
        return ResultSuccess();
    }
    Result ParseInstSLLIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(static_cast<WordS>(rs1.Get<Word>() << imm.Get<Word>()));
        return ResultSuccess();
    }
    Result ParseInstSRLIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(static_cast<WordS>(rs1.Get<Word>() >> imm.Get<Word>()));
        return ResultSuccess();
    }
    Result ParseInstSRAIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        rd.Set(rs1.Get<WordS>() >> imm.Get<Word>());
        return ResultSuccess();
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode STORE.
     */
    Result ParseInstSB(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemoryWriteByte(rs2.Get<Byte>(), rs1.Get<Address>() + imm.Get<Address>());
    }
    Result ParseInstSH(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemoryWriteHWord(rs2.Get<HWord>(), rs1.Get<Address>() + imm.Get<Address>());
    }
    Result ParseInstSW(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemoryWriteWord(rs2.Get<Word>(), rs1.Get<Address>() + imm.Get<Address>());
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstSD(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemoryWriteDWord(rs2.Get<DWord>(), rs1.Get<Address>() + imm.Get<Address>());
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode OP.
     */
    Result ParseInstADD(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() + rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstSUB(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() - rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstMUL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstSLL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() << (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstMULH(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstSLT(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() < rs2.Get<NativeWordS>());
        return ResultSuccess();
    }
    Result ParseInstMULHSU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstSLTU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() < rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstMULHU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstXOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() ^ rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstDIV(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstSRL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() >> (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstSRA(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() >> (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstDIVU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() | rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstREM(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstAND(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() & rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstREMU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }

#ifdef RISCV_CFG_CPU_ENABLE_RV64
    /*
     * Opcode OP_32.
     */
    Result ParseInstADDW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<WordS>() + rs2.Get<WordS>());
        return ResultSuccess();
    }
    Result ParseInstSUBW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<WordS>() - rs2.Get<WordS>());
        return ResultSuccess();
    }
    Result ParseInstMULW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstSLLW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(static_cast<WordS>(rs1.Get<Word>() << (rs2.Get<Word>() & ShiftAmtMaskFor32)));
        return ResultSuccess();
    }
    Result ParseInstDIVW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstSRLW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(static_cast<WordS>(rs1.Get<Word>() >> (rs2.Get<Word>() & ShiftAmtMaskFor32)));
        return ResultSuccess();
    }
    Result ParseInstSRAW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<WordS>() >> (rs2.Get<Word>() & ShiftAmtMaskFor32));
        return ResultSuccess();
    }
    Result ParseInstDIVUW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstREMW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
    Result ParseInstREMUW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return ResultNotImplemented();
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode LUI.
     */
    Result ParseInstLUI(OutRegObject rd, ImmediateObject imm) {
        rd.Set(imm.Get<NativeWord>());
        return ResultSuccess();
    }

    /*
     * Opcode BRANCH.
     */
    Result InstBranchImpl(bool cond, ImmediateObject imm) {
        if(cond) {
            return m_pParent->SignalBranch(imm.Get<Address>());
        }
        return ResultSuccess();
    }
    Result ParseInstBEQ(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return InstBranchImpl(rs1.Get<NativeWord>() == rs2.Get<NativeWord>(), imm);
    }
    Result ParseInstBNE(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return InstBranchImpl(rs1.Get<NativeWord>() != rs2.Get<NativeWord>(), imm);
    }
    Result ParseInstBLT(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return InstBranchImpl(rs1.Get<NativeWordS>() < rs2.Get<NativeWordS>(), imm);
    }
    Result ParseInstBGE(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return InstBranchImpl(rs1.Get<NativeWordS>() >= rs2.Get<NativeWordS>(), imm);
    }
    Result ParseInstBLTU(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return InstBranchImpl(rs1.Get<NativeWord>() < rs2.Get<NativeWord>(), imm);
    }
    Result ParseInstBGEU(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return InstBranchImpl(rs1.Get<NativeWord>() >= rs2.Get<NativeWord>(), imm);
    }

    /*
     * Opcode JALR.
     */
    Result ParseInstJALR(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        /* Save current PC to rd. */
        rd.Set(m_pParent->m_PC + 4);

        /* Signal jump. */
        return m_pParent->SignalJump((rs1.Get<Address>() + imm.Get<Address>()) & ~static_cast<Address>(1u));
    }

    /*
     * Opcode JAL.
     */
    Result ParseInstJAL(OutRegObject rd, ImmediateObject imm) {
        /* Save current PC to rd. */
        rd.Set(m_pParent->m_PC + 4);

        /* Signal branch. */
        return m_pParent->SignalBranch(imm.Get<Address>());
    }
private:
    Hart* const m_pParent = 0;
}; // class Hart::InstructionRunner

Result Hart::ExecuteInstruction(Instruction inst) {
    return InstructionRunner(this).ParseInstruction(inst);
}

} // namespace cpu
} // namespace riscv
