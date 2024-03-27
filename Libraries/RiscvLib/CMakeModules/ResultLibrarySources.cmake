set(_RV_RESULT_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/result")
#set(_RV_RESULT_SRC_DIR "${RISCVLIB_SOURCE_DIR}/result")

set(RISCV_RESULT_LIBRARY_HEADERS
    "${RISCVLIB_HEADER_DIR}/RiscvEmu/result.h"

    "${_RV_RESULT_HDR_DIR}/result_ErrorBase.h"
    "${_RV_RESULT_HDR_DIR}/result_Result.h"
    "${_RV_RESULT_HDR_DIR}/result_ResultSuccess.h"

    "${_RV_RESULT_HDR_DIR}/detail/result_ResultBase.h"
    "${_RV_RESULT_HDR_DIR}/detail/result_ResultValue.h"
)

set(RISCV_RESULT_LIBRARY_SOURCES
    # ... 
)
