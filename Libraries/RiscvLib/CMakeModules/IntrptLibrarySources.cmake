set(_RV_INTRPT_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/intrpt")
set(_RV_INTRPT_SRC_DIR "${RISCVLIB_SOURCE_DIR}/intrpt")

set(RISCV_INTRPT_LIBRARY_HEADERS
    "${RISCVLIB_HEADER_DIR}/RiscvEmu/intrpt.h"

    "${_RV_INTRPT_HDR_DIR}/intrpt_ISource.h"
    "${_RV_INTRPT_HDR_DIR}/intrpt_ITarget.h"
    "${_RV_INTRPT_HDR_DIR}/intrpt_PLIC.h"
    "${_RV_INTRPT_HDR_DIR}/intrpt_Result.h"
    "${_RV_INTRPT_HDR_DIR}/detail/intrpt_ITargetForCtrl.h"
)

set(RISCV_INTRPT_LIBRARY_SOURCES
    "${_RV_INTRPT_SRC_DIR}/intrpt_ITarget.cpp"
    "${_RV_INTRPT_SRC_DIR}/intrpt_PLIC.cpp"
    "${_RV_INTRPT_SRC_DIR}/detail/intrpt_ITargetForCtrl.cpp"
)
