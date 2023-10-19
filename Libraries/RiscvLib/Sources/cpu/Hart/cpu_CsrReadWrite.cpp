#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/cpu/cpu_CsrFormat.h>

namespace riscv {
namespace cpu {

namespace {

constexpr bool CanWrite(CsrId id) noexcept {
    return static_cast<int>(id) >> 8 & 0x3;
}

constexpr bool CanAccess(CsrId id, PrivilageLevel level) noexcept {
    return (static_cast<int>(id) >> 10 & 0x3) >= static_cast<int>(level);
}

} // namespace

Result Hart::RwmCSRImpl(NativeWord* pOut, NativeWord writeVal, RwmCSRReadFunc readFunc, RwmCSRWriteFunc writeFunc, CsrMakeValFunc makeValFunc) {
    Result res;

    /* Read the CSR. */
    res = (*this.*readFunc)(pOut);
    if(res.IsFailure()) {
        return res;
    }

    /* Create new value. */
    writeVal = makeValFunc(*pOut, writeVal);

    /* Write new value. */
    return (*this.*writeFunc)(writeVal);
}

Result Hart::ReadWriteCSRImpl(CsrId id, NativeWord* pOut, NativeWord writeVal, CsrMakeValFunc makeValFunc) {
    /* Make sure we can access this register from our current privilage level. */
    if(!CanAccess(id, m_CurPrivLevel)) {
        return ResultCsrPrivilageTooLow();
    }

    /* If we're writing, make sure this register is writable. */
    if(makeValFunc && !CanWrite(id)) {
        return ResultWriteReadOnlyCsr();
    }

    switch(id) {
    case CsrId::sscratch:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_sscratch, &Hart::CSRWrite_sscratch, makeValFunc);
    case CsrId::satp:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_satp, &Hart::CSRWrite_satp, makeValFunc);
    case CsrId::mscratch:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_mscratch, &Hart::CSRWrite_mscratch, makeValFunc);
    case CsrId::mcycle:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_mcycle, &Hart::CSRWrite_mcycle, makeValFunc);
    case CsrId::minstret:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_minstret, &Hart::CSRWrite_minstret, makeValFunc);
    case CsrId::mcycleh:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_mcycleh, &Hart::CSRWrite_mcycleh, makeValFunc);
    case CsrId::minstreth:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_minstreth, &Hart::CSRWrite_minstreth, makeValFunc);
    case CsrId::cycle:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_cycle, nullptr, nullptr);
    case CsrId::instret:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_instret, nullptr, nullptr);
    case CsrId::cycleh:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_cycleh, nullptr, nullptr);
    case CsrId::instreth:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRRead_instreth, nullptr, nullptr);
    
    default: break;
    }

    return ResultCsrIdInvalid();
}

Result Hart::CSRRead_sscratch(NativeWord* pOut) {
    *pOut = m_SupervisorScratch;
    return ResultSuccess();
}

Result Hart::CSRWrite_sscratch(NativeWord val) {
    m_SupervisorScratch = val;
    return ResultSuccess();
}

Result Hart::CSRRead_satp(NativeWord* pOut) {
    /* Create value. */
    *pOut = csr::satp().SetPPN(m_MemMgr.GetPTAddr() >> 12)
        .SetASID(m_MemMgr.GetASID())
        .SetMODE(m_MemMgr.GetTransMode())
        .GetValue();

    return ResultSuccess();
}

Result Hart::CSRWrite_satp(NativeWord val) {
    /* Give values to MemoryManager. */
    csr::satp fmt(val);
    m_MemMgr.SetPTAddr(fmt.GetPPN() << 12);
    m_MemMgr.SetASID(fmt.GetASID());
    
    return m_MemMgr.SetTransMode(fmt.GetMODE());
}

Result Hart::CSRRead_mscratch(NativeWord* pOut) {
    *pOut = m_MachineScratch;
    return ResultSuccess();
}

Result Hart::CSRWrite_mscratch(NativeWord val) {
    m_MachineScratch = val;
    return ResultSuccess();
}

Result Hart::CSRRead_mcycle(NativeWord* pOut) {
    /* Read lower 32bits on RV32, full 64bits on RV64. */
    *pOut = static_cast<NativeWord>(m_CycleCount);
    return ResultSuccess();
}

Result Hart::CSRWrite_mcycle(NativeWord in) {
    if constexpr(cfg::cpu::EnableIsaRV64I) {
        /* On RV64, we write to the cycle counter directly. */
        m_CycleCount = static_cast<DWord>(in);
    }
    else {
        /* On RV32, we replace the lower bits with the provided value. */
        m_CycleCount = (m_CycleCount & ~0xFFFFFFFF) | in;
    }

    return ResultSuccess();
}

Result Hart::CSRRead_mcycleh(NativeWord* pOut) {
    /* This CSR doesn't exits on RV64. */
    if constexpr(cfg::cpu::EnableIsaRV64I) {
        return ResultCsrIdInvalid();
    }

    /* Read upper 32bits. */
    *pOut = static_cast<NativeWord>(m_CycleCount >> 32);

    return ResultSuccess();
}

Result Hart::CSRWrite_mcycleh(NativeWord in) {
    /* This CSR doesn't exist on RV64. */
    if constexpr(cfg::cpu::EnableIsaRV64I) {
        return ResultCsrIdInvalid();
    }

    /* Replace the upper bits with the provided value. */
    m_CycleCount = (m_CycleCount & 0xFFFFFFFF) | (static_cast<DWord>(in) << 32);

    return ResultSuccess();
}

Result Hart::CSRRead_minstret(NativeWord* pOut) {
    /* This is functionally the same as read_mcycle on our impl. */
    return this->CSRRead_mcycle(pOut);
}

Result Hart::CSRWrite_minstret(NativeWord in) {
    /* This is functionally the same as write_mcycle on our impl. */
    return this->CSRWrite_mcycle(in);
}

Result Hart::CSRRead_minstreth(NativeWord* pOut) {
    /* This is functionally the same as read_mcycleh on our impl. */
    return this->CSRRead_mcycleh(pOut);
}

Result Hart::CSRWrite_minstreth(NativeWord in) {
    /* This is functionally the same as write_mcycleh on our impl. */
    return this->CSRWrite_mcycleh(in);
}

Result Hart::CSRRead_cycle(NativeWord* pOut) {
    /* This is the same as reading mcycle. */
    return this->CSRRead_mcycle(pOut);
}

Result Hart::CSRRead_cycleh(NativeWord* pOut) {
    /* This is the same as reading mcycleh. */
    return this->CSRRead_mcycleh(pOut);
}

Result Hart::CSRRead_instret(NativeWord* pOut) {
    /* This is the same as reading minstret. */
    return this->CSRRead_minstret(pOut);
}

Result Hart::CSRRead_instreth(NativeWord* pOut) {
    /* This is the same as reading minstreth. */
    return this->CSRRead_minstreth(pOut);
}

} // namespace cpu
} // namespace riscv
