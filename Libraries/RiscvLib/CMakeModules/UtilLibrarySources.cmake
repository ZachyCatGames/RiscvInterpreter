set(_RV_UTIL_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/util")
#set(_RV_UTIL_SRC_DIR "${RISCVLIB_SOURCE_DIR}/util")

set(RISCV_UTIL_LIBRARY_HEADERS
    "${_RV_UTIL_HDR_DIR}/util_Alignment.h"
    "${_RV_UTIL_HDR_DIR}/util_Bitfields.h"
    "${_RV_UTIL_HDR_DIR}/util_Bitmask.h"
    "${_RV_UTIL_HDR_DIR}/util_BitSwap.h"
    "${_RV_UTIL_HDR_DIR}/util_ByteCount.h"
    "${_RV_UTIL_HDR_DIR}/util_OverflowCheck.h"
    "${_RV_UTIL_HDR_DIR}/util_SignExtend.h"
)

set(RISCV_UTIL_LIBRARY_SOURCES
    # ...
)
