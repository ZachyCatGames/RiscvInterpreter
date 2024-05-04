#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/cpu/cpu_Values.h>
#include <RiscvEmu/cpu/detail/cpu_DecoderImpl.h>
#include <RiscvEmu/cpu/detail/cpu_IntegerMultiply.h>
#include <bit>
#include <concepts>

namespace riscv {
namespace cpu {

namespace {

constexpr NativeWord MakeValForCSRRW([[maybe_unused]] NativeWord, NativeWord writeVal) noexcept {
    return writeVal;
}

constexpr NativeWord MakeValForCSRRS(NativeWord curVal, NativeWord writeVal) noexcept {
    return curVal | writeVal;
}

constexpr NativeWord MakeValForCSRRC(NativeWord curVal, NativeWord writeVal) noexcept {
    return curVal & ~writeVal;
}

} // namespace

class Hart::InstructionRunner : public detail::DecoderImpl<Hart::InstructionRunner> {
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
    Result InstLoadImpl(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        /* Perform load. */
        T out = 0;
        Result res = m_pParent->MemLoad(&out, rs1.Get<Address>() + imm.Get<Address>());

        if(res.IsSuccess()) {
            /* Sign extend if needed, write to output register. */
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
        return this->InstLoadImpl<true, Byte>(rd, rs1, imm);
    }
    Result ParseInstLH(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, HWord>(rd, rs1, imm);
    }
    Result ParseInstLW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, Word>(rd, rs1, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstLD(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<true, DWord>(rd, rs1, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstLBU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, Byte>(rd, rs1, imm);
    }
    Result ParseInstLHU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, HWord>(rd, rs1, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstLWU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->InstLoadImpl<false, Word>(rd, rs1, imm);
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
        return m_pParent->MemStore(rs2.Get<Byte>(), rs1.Get<Address>() + imm.Get<Address>());
    }
    Result ParseInstSH(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemStore(rs2.Get<HWord>(), rs1.Get<Address>() + imm.Get<Address>());
    }
    Result ParseInstSW(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemStore(rs2.Get<Word>(), rs1.Get<Address>() + imm.Get<Address>());
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    Result ParseInstSD(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return m_pParent->MemStore(rs2.Get<DWord>(), rs1.Get<Address>() + imm.Get<Address>());
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
        rd.Set(rs1.Get<NativeWord>() * rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstSLL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() << (rs2.Get<Word>() & ShiftAmtMask));
        return ResultSuccess();
    }
    Result ParseInstMULH(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        if constexpr(cfg::cpu::EnableIsaRV64I) {
            rd.Set(detail::MultiplyGetUpper64S(rs1.Get<DWordS>(), rs2.Get<DWordS>()));
        }
        else {
            rd.Set(static_cast<Word>(rs1.Get<DWordS>() * rs2.Get<DWordS>() >> 32));
        }
        return ResultSuccess();
    }
    Result ParseInstSLT(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() < rs2.Get<NativeWordS>());
        return ResultSuccess();
    }
    Result ParseInstMULHSU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        if constexpr(cfg::cpu::EnableIsaRV64I) {
            rd.Set(detail::MultiplyGetUpper64SU(rs1.Get<DWordS>(), rs2.Get<DWord>()));
        }
        else {
            rd.Set(static_cast<Word>(rs1.Get<DWordS>() * static_cast<DWordS>(rs2.Get<DWord>()) >> 32));
        }
        return ResultSuccess();
    }
    Result ParseInstSLTU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() < rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstMULHU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        if constexpr(cfg::cpu::EnableIsaRV64I) {
            rd.Set(detail::MultiplyGetUpper64U(rs1.Get<DWord>(), rs2.Get<DWord>()));
        }
        else {
            rd.Set(static_cast<Word>(rs1.Get<DWord>() * rs2.Get<DWord>() >> 32));
        }
        return ResultSuccess();
    }
    Result ParseInstXOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() ^ rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstDIV(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() / rs2.Get<NativeWordS>());
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
    Result ParseInstDIVU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() / rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() | rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstREM(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWordS>() % rs2.Get<NativeWordS>());
        return ResultSuccess();
    }
    Result ParseInstAND(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() & rs2.Get<NativeWord>());
        return ResultSuccess();
    }
    Result ParseInstREMU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<NativeWord>() % rs2.Get<NativeWord>());
        return ResultSuccess();
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
        rd.Set(rs1.Get<WordS>() * rs2.Get<WordS>());
        return ResultSuccess();
    }
    Result ParseInstSLLW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(static_cast<WordS>(rs1.Get<Word>() << (rs2.Get<Word>() & ShiftAmtMaskFor32)));
        return ResultSuccess();
    }
    Result ParseInstDIVW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<WordS>() / rs2.Get<WordS>());
        return ResultSuccess();
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
        rd.Set(rs1.Get<Word>() / rs2.Get<Word>());
        return ResultSuccess();
    }
    Result ParseInstREMW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<WordS>() % rs2.Get<WordS>());
        return ResultSuccess();
    }
    Result ParseInstREMUW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        rd.Set(rs1.Get<Word>() % rs2.Get<Word>());
        return ResultSuccess();
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

    /*
     * Opcode SYSTEM.
     */
    Result ParseInstCSRRW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        NativeWord val = 0;
        
        Result res = m_pParent->ReadWriteCSRImpl(static_cast<CsrId>(imm.Get<int>()), &val, rs1.Get<NativeWord>(), &MakeValForCSRRW);
        rd.Set(val);

        return res;
    }
    Result ParseInstCSRRS(OutRegObject rd, InRegObject rs1, ImmediateObject imm, ImmediateObject rsId) {
        NativeWord val = 0;
        auto makeValFunc = rsId.Get<int>() == 0 ? nullptr : &MakeValForCSRRS;

        Result res = m_pParent->ReadWriteCSRImpl(static_cast<CsrId>(imm.Get<int>()), &val, rs1.Get<NativeWord>(), makeValFunc);
        rd.Set(val);

        return res;
    }
    Result ParseInstCSRRC(OutRegObject rd, InRegObject rs1, ImmediateObject imm, ImmediateObject rsId) {
        NativeWord val = 0;
        auto makeValFunc = rsId.Get<int>() == 0 ? nullptr : &MakeValForCSRRC;

        Result res = m_pParent->ReadWriteCSRImpl(static_cast<CsrId>(imm.Get<int>()), &val, rs1.Get<NativeWord>(), makeValFunc);
        rd.Set(val);

        return res;
    }
    Result ParseInstCSRRWI(OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        NativeWord val = 0;
        
        Result res = m_pParent->ReadWriteCSRImpl(static_cast<CsrId>(csr.Get<int>()), &val, src.Get<NativeWord>(), &MakeValForCSRRW);
        rd.Set(val);

        return res;
    }
    Result ParseInstCSRRSI(OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        NativeWord val = 0;
        auto makeValFunc = src.Get<NativeWord>() == 0 ? nullptr : &MakeValForCSRRS;

        Result res = m_pParent->ReadWriteCSRImpl(static_cast<CsrId>(csr.Get<int>()), &val, src.Get<NativeWord>(), makeValFunc);
        rd.Set(val);

        return res;
    }
    Result ParseInstCSRRCI(OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        NativeWord val = 0;
        auto makeValFunc = src.Get<NativeWord>() == 0 ? nullptr : &MakeValForCSRRC;

        Result res = m_pParent->ReadWriteCSRImpl(static_cast<CsrId>(csr.Get<int>()), &val, src.Get<NativeWord>(), makeValFunc);
        rd.Set(val);

        return res;
    }
private:
    Hart* const m_pParent = 0;
}; // class Hart::InstructionRunner

Result Hart::ExecuteInstructionImpl(Instruction inst) {
    /* Clear X0 incase the previous instruction wrote to it. */
    m_GPR[0] = 0;

    Result res = InstructionRunner(this).ParseInstruction(inst);

    /* Increment cycle counter. */
    ++m_CycleCount;

    return res;
}

} // namespace cpu
} // namespace riscv
