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
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRReadSscratch, &Hart::CSRWriteSscratch, makeValFunc);
    case CsrId::satp:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRReadSatp, &Hart::CSRWriteSatp, makeValFunc);
    case CsrId::mscratch:
        return this->RwmCSRImpl(pOut, writeVal, &Hart::CSRReadMscratch, &Hart::CSRWriteMscratch, makeValFunc);
    default: break;
    }

    return ResultCsrIdInvalid();
}

Result Hart::CSRReadSscratch(NativeWord* pOut) {
    *pOut = m_SupervisorScratch;
    return ResultSuccess();
}

Result Hart::CSRWriteSscratch(NativeWord val) {
    m_SupervisorScratch = val;
    return ResultSuccess();
}

Result Hart::CSRReadSatp(NativeWord* pOut) {
    /* Create value. */
    *pOut = csr::satp().SetPPN(m_MemMgr.GetPTAddr() >> 12)
        .SetASID(m_MemMgr.GetASID())
        .SetMODE(m_MemMgr.GetTransMode())
        .GetValue();

    return ResultSuccess();
}

Result Hart::CSRWriteSatp(NativeWord val) {
    /* Give values to MemoryManager. */
    csr::satp fmt(val);
    m_MemMgr.SetPTAddr(fmt.GetPPN() << 12);
    m_MemMgr.SetASID(fmt.GetASID());
    
    return m_MemMgr.SetTransMode(fmt.GetMODE());
}

Result Hart::CSRReadMscratch(NativeWord* pOut) {
    *pOut = m_MachineScratch;
    return ResultSuccess();
}

Result Hart::CSRWriteMscratch(NativeWord val) {
    m_MachineScratch = val;
    return ResultSuccess();
}

Result Hart::CSRReadTime(NativeWord* pOut) {
    DWord time = 0;
    m_ClkTime.GetCurrentTime(&time);
    *pOut = static_cast<NativeWord>(time);
    return ResultSuccess();
}

Result Hart::CSRReadTimeh(Word* pOut) {
    if constexpr(cfg::cpu::EnableIsaRV64I) {
        return ResultCsrIdInvalid();
    }

    /* Read upper half of time. */
    DWord time = 0;
    m_ClkTime.GetCurrentTime(&time);
    *pOut = static_cast<Word>(time >> 32);
    return ResultSuccess();
}

} // namespace cpu
} // namespace riscv
