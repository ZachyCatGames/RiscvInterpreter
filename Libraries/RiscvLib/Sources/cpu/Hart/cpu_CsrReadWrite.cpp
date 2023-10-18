#include <RiscvEmu/cpu/cpu_Hart.h>

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
    default: break;
    }

    return ResultCsrIdInvalid();
}

} // namespace cpu
} // namespace riscv
