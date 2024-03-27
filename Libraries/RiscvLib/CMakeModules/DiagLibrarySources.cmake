set(_RV_DIAG_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/diag")
set(_RV_DIAG_SRC_DIR "${RISCVLIB_SOURCE_DIR}/diag")

set(RISCV_DIAG_LIBRARY_HEADERS
    "${RISCVLIB_HEADER_DIR}/RiscvEmu/diag.h"

    "${_RV_DIAG_HDR_DIR}/diag_Abort.h"
    "${_RV_DIAG_HDR_DIR}/diag_Assert.h"
    "${_RV_DIAG_HDR_DIR}/diag_DebugLog.h"
    "${_RV_DIAG_HDR_DIR}/diag_FormatString.h"
    "${_RV_DIAG_HDR_DIR}/diag_Log.h"

    "${_RV_DIAG_HDR_DIR}/detail/diag_AbortImpl.h"
    "${_RV_DIAG_HDR_DIR}/detail/diag_AssertImpl.h"
    "${_RV_DIAG_HDR_DIR}/detail/diag_DebugLogImpl.h"
    "${_RV_DIAG_HDR_DIR}/detail/diag_LogImpl.h"
    "${_RV_DIAG_HDR_DIR}/detail/diag_PrintSourceLocation.h"
)

set(RISCV_DIAG_LIBRARY_SOURCES
    "${_RV_DIAG_SRC_DIR}/diag_Abort.cpp"

    "${_RV_DIAG_SRC_DIR}/detail/diag_AbortImpl.cpp"
    "${_RV_DIAG_SRC_DIR}/detail/diag_AssertImpl.cpp"
    "${_RV_DIAG_SRC_DIR}/detail/diag_DebugLogImpl.cpp"
    "${_RV_DIAG_SRC_DIR}/detail/diag_LogImpl.cpp"
    "${_RV_DIAG_SRC_DIR}/detail/diag_PrintSourceLocation.cpp"
)
