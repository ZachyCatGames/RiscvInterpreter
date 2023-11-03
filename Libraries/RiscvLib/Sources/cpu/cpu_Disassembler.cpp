#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/cpu/cpu_Disassembler.h>
#include <RiscvEmu/cpu/detail/cpu_DecoderImpl.h>
#include <concepts>
#include <format>

namespace riscv {
namespace cpu {

namespace {

class DisassemblerImpl : private detail::DecoderImpl<DisassemblerImpl> {
public:
    constexpr DisassemblerImpl() noexcept = default;
    
    constexpr std::string Disassemble(Instruction inst) {
        /* Parse instruction. */
        Result res = this->ParseInstruction(inst);

        /* On failure, return "unknown." */
        if(res.IsFailure()) {
            return "unknown";
        }

        /* Otherwise return internal string. */
        return m_StrTmp;
    }
private:
    friend class DecoderImpl<DisassemblerImpl>;

    class RegObject {
    public:
        constexpr RegObject(int id) noexcept :
            m_Id(id) {}

        constexpr int GetId() const noexcept { return m_Id; }
    private:
        int m_Id;
    };

    using InRegObject = RegObject;
    using OutRegObject = RegObject;

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

    constexpr InRegObject CreateInRegImpl(int id) const noexcept {
        return InRegObject(id);
    }

    constexpr OutRegObject CreateOutRegImpl(int id) const noexcept {
        return OutRegObject(id);
    }

    constexpr ImmediateObject CreateImmediateImpl(NativeWord val) const noexcept {
        return ImmediateObject(val);
    }

    constexpr Result FormatStandardRType(std::string_view name, OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        m_StrTmp = std::format("{} x{}, x{}, x{}", name, rd.GetId(), rs1.GetId(), rs2.GetId());
        return ResultSuccess();
    }

    constexpr Result FormatStandardIType(std::string_view name, OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, x{}, {}", name, rd.GetId(), rs1.GetId(), imm.Get<Word>());
        return ResultSuccess();
    }

    constexpr Result FormatStandardITypeExt(std::string_view name, OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, x{}, {}", name, rd.GetId(), rs1.GetId(), imm.Get<WordS>());
        return ResultSuccess();
    }

    constexpr Result FormatStandardSTypeExt(std::string_view name, InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, x{}, {}", name, rs1.GetId(), rs2.GetId(), imm.Get<WordS>());
        return ResultSuccess();
    }

    constexpr Result FormatStandardBTypeExt(std::string_view name, InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, x{}, {}", name, rs1.GetId(), rs2.GetId(), imm.Get<WordS>());
        return ResultSuccess();
    }

    constexpr Result FormatStandardUTypeExt(std::string_view name, OutRegObject rd, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, {}", name, rd.GetId(), imm.Get<WordS>());
        return ResultSuccess();
    }

    constexpr Result FormatStandardJTypeExt(std::string_view name, OutRegObject rd, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, {}", name, rd.GetId(), imm.Get<WordS>());
        return ResultSuccess();
    }

    /*
     * Opcode LOAD.
     */
    constexpr Result FormatLOAD(std::string_view name, OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, {}(x{})", name, rd.GetId(), imm.Get<WordS>(), rs1.GetId());
        return ResultSuccess();
    }
    constexpr Result ParseInstLB(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LB", rd, rs1, imm);
    }
    constexpr Result ParseInstLH(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LH", rd, rs1, imm);
    }
    constexpr Result ParseInstLW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LW", rd, rs1, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    constexpr Result ParseInstLD(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LD", rd, rs1, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64
    constexpr Result ParseInstLBU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LBU", rd, rs1, imm);
    }
    constexpr Result ParseInstLHU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LHU", rd, rs1, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    constexpr Result ParseInstLWU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatLOAD("LWU", rd, rs1, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode MISC_MEM.
     */
    constexpr Result ParseInstFENCE([[maybe_unused]] OutRegObject rd, [[maybe_unused]] InRegObject rs1, [[maybe_unused]] ImmediateObject imm) {
        return ResultSuccess();
    }
    constexpr Result ParseInstFENCEI([[maybe_unused]] OutRegObject rd, [[maybe_unused]] InRegObject rs1, [[maybe_unused]] ImmediateObject imm) {
        m_StrTmp = "FENCE.I";
        return ResultSuccess();
    }

    /*
     * Opcode OP_IMM.
     */
    constexpr Result ParseInstADDI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("ADDI", rd, rs1, imm);
    }
    constexpr Result ParseInstSLLI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SLLI", rd, rs1, imm);
    }
    constexpr Result ParseInstSLTI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SLTI", rd, rs1, imm);
    }
    constexpr Result ParseInstSLTIU(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SLTIU", rd, rs1, imm);
    }
    constexpr Result ParseInstXORI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("XORI", rd, rs1, imm);
    }
    constexpr Result ParseInstSRLI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SRLI", rd, rs1, imm);
    }
    constexpr Result ParseInstSRAI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SRAI", rd, rs1, imm);
    }
    constexpr Result ParseInstORI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("ORI", rd, rs1, imm);
    }
    constexpr Result ParseInstANDI(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("ANDI", rd, rs1, imm);
    }

    /*
     * Opcode AUIPC.
     */
    constexpr Result ParseInstAUIPC(OutRegObject rd, ImmediateObject imm) {
        return this->FormatStandardUTypeExt("AUIPC", rd, imm);
    }

#ifdef RISCV_CFG_CPU_ENABLE_RV64
    /*
     * Opcode OP_IMM_32.
     */
    constexpr Result ParseInstADDIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("ADDIW", rd, rs1, imm);
    }
    constexpr Result ParseInstSLLIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SLLIW", rd, rs1, imm);
    }
    constexpr Result ParseInstSRLIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SRLIW", rd, rs1, imm);
    }
    constexpr Result ParseInstSRAIW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("SRAIW", rd, rs1, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode STORE.
     */
    constexpr Result FormatSTORE(std::string_view name, InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        m_StrTmp = std::format("{} x{}, {}(x{})", name, rs2.GetId(), imm.Get<WordS>(), rs1.GetId());
        return ResultSuccess();
    }
    constexpr Result ParseInstSB(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatSTORE("SB", rs1, rs2, imm);
    }
    constexpr Result ParseInstSH(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatSTORE("SH", rs1, rs2, imm);
    }
    constexpr Result ParseInstSW(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatSTORE("SW", rs1, rs2, imm);
    }
#ifdef RISCV_CFG_CPU_ENABLE_RV64
    constexpr Result ParseInstSD(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatSTORE("SD", rs1, rs2, imm);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode OP.
     */
    constexpr Result ParseInstADD(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("ADD", rd, rs1, rs2);
    }
    constexpr Result ParseInstSUB(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SUB", rd, rs1, rs2);
    }
    constexpr Result ParseInstMUL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("MUL", rd, rs1, rs2);
    }
    constexpr Result ParseInstSLL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SLL", rd, rs1, rs2);
    }
    constexpr Result ParseInstMULH(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("MULH", rd, rs1, rs2);
    }
    constexpr Result ParseInstSLT(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SLT", rd, rs1,rs2);
    }
    constexpr Result ParseInstMULHSU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("MULHSU", rd, rs1,rs2);
    }
    constexpr Result ParseInstSLTU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SLTU", rd, rs1, rs2);
    }
    constexpr Result ParseInstMULHU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("MULHU", rd, rs1, rs2);
    }
    constexpr Result ParseInstXOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("XOR", rd, rs1, rs2);
    }
    constexpr Result ParseInstDIV(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("DIV", rd, rs1, rs2);
    }
    constexpr Result ParseInstSRL(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SRL", rd, rs1, rs2);
    }
    constexpr Result ParseInstSRA(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SRA", rd, rs1, rs2);
    }
    constexpr Result ParseInstDIVU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("DIVU", rd, rs1, rs2);
    }
    constexpr Result ParseInstOR(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("OR", rd, rs1, rs2);
    }
    constexpr Result ParseInstREM(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("REM", rd, rs1, rs2);
    }
    constexpr Result ParseInstAND(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("AND", rd, rs1, rs2);
    }
    constexpr Result ParseInstREMU(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("REMU", rd, rs1, rs2);
    }

    /*
     * Opcode LUI.
     */
    constexpr Result ParseInstLUI(OutRegObject rd, ImmediateObject imm) {
        return this->FormatStandardUTypeExt("LUI", rd, imm);
    }

#ifdef RISCV_CFG_CPU_ENABLE_RV64
    /*
     * Opcode OP_32.
     */
    constexpr Result ParseInstADDW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("ADDW", rd, rs1, rs2);
    }
    constexpr Result ParseInstSUBW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SUBW", rd, rs1, rs2);
    }
    constexpr Result ParseInstMULW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("MULW", rd, rs1, rs2);
    }
    constexpr Result ParseInstSLLW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SLLW", rd, rs1, rs2);
    }
    constexpr Result ParseInstDIVW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("DIVW", rd, rs1, rs2);
    }
    constexpr Result ParseInstSRLW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SRLW", rd, rs1, rs2);
    }
    constexpr Result ParseInstSRAW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("SRAW", rd, rs1, rs2);
    }
    constexpr Result ParseInstDIVUW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("DIVUW", rd, rs1, rs2);
    }
    constexpr Result ParseInstREMW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("REMW", rd, rs1, rs2);
    }
    constexpr Result ParseInstREMUW(OutRegObject rd, InRegObject rs1, InRegObject rs2) {
        return this->FormatStandardRType("REMUW", rd, rs1, rs2);
    }
#endif // RISCV_CFG_CPU_ENABLE_RV64

    /*
     * Opcode BRANCH.
     */
    constexpr Result ParseInstBEQ(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatStandardBTypeExt("BEQ", rs1, rs2, imm);
    }
    constexpr Result ParseInstBNE(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatStandardBTypeExt("BNE", rs1, rs2, imm);
    }
    constexpr Result ParseInstBLT(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatStandardBTypeExt("BLT", rs1, rs2, imm);
    }
    constexpr Result ParseInstBGE(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatStandardBTypeExt("BGE", rs1, rs2, imm);
    }
    constexpr Result ParseInstBLTU(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatStandardBTypeExt("BLTU", rs1, rs2, imm);
    }
    constexpr Result ParseInstBGEU(InRegObject rs1, InRegObject rs2, ImmediateObject imm) {
        return this->FormatStandardBTypeExt("BGEU", rs1, rs2, imm);
    }

    /*
     * Opcode JALR.
     */
    constexpr Result ParseInstJALR(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardITypeExt("JALR", rd, rs1, imm);
    }

    /*
     * Opcode JAL.
     */
    constexpr Result ParseInstJAL(OutRegObject rd, ImmediateObject imm) {
        return this->FormatStandardJTypeExt("JAL", rd, imm);
    }

    /*
     * Opcode SYSTEM.
     */
    constexpr Result FormatCSRI(std::string_view name, OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        m_StrTmp = std::format("{} x{}, {}, {}", name, rd.GetId(), src.Get<Word>(), csr.Get<Word>());
        return ResultSuccess();
    }
    constexpr Result ParseInstCSRRW(OutRegObject rd, InRegObject rs1, ImmediateObject imm) {
        return this->FormatStandardIType("CSRRW", rd, rs1, imm);
    }
    constexpr Result ParseInstCSRRS(OutRegObject rd, InRegObject rs1, ImmediateObject imm, [[maybe_unused]] ImmediateObject rsId) {
        return this->FormatStandardIType("CSRRS", rd, rs1, imm);
    }
    constexpr Result ParseInstCSRRC(OutRegObject rd, InRegObject rs1, ImmediateObject imm, [[maybe_unused]] ImmediateObject rsId) {
        return this->FormatStandardIType("CSRRC", rd, rs1, imm);
    }
    constexpr Result ParseInstCSRRWI(OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        return this->FormatCSRI("CSRRWI", rd, src, csr);
    }
    constexpr Result ParseInstCSRRSI(OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        return this->FormatCSRI("CSRRSI", rd, src, csr);
    }
    constexpr Result ParseInstCSRRCI(OutRegObject rd, ImmediateObject src, ImmediateObject csr) {
        return this->FormatCSRI("CSRRCI", rd, src, csr);
    }

private:
    std::string m_StrTmp;
}; // class DisassemblerImpl

} // namespace

std::string DisassembleInstruction(Instruction inst) {
    return DisassemblerImpl().Disassemble(inst);
}

} // namespace cpu
} // namespace riscv
