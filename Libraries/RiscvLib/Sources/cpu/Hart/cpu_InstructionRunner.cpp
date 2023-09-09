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
    Result ParseInstLBU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, Byte>(&Hart::MemoryReadByte, rd, rs1, imm);
    }
    Result ParseInstLHU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, HWord>(&Hart::MemoryReadHWord, rd, rs1, imm);
    }

    /*
     * Opcode MISC_MEM.
     */
    Result ParseInstFENCE(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        /* 
         * We're always going to be executing instructions in-order,
         * so FENCE doesn't need to be implemented.
         */
        (void)rd;
        (void)rs1;
        (void)imm;
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
        rd.Set(rs1.Get<NativeWord>() >> imm.Get<NativeWord>());
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
    Result ParseInstSLL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() << (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstSLT(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() < rs2.Get<NativeWordS>());
        return ResultSuccess();
    }
    Result ParseInstSLTU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() < rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstXOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() ^ rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstSRL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() >> (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstSRA(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() >> (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() | rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstAND(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() & rs2.Get<NativeWord>());
        return ResultSuccess();
    }

    /*
     * Opcode LUI.
     */
    Result ParseInstLUI(OutRegObject rd, ImmediateObject imm) {
        rd.Set(imm.Get<Word>());
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
