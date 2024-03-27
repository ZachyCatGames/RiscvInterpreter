set(_RV_HW_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/hw")
set(_RV_HW_SRC_DIR "${RISCVLIB_SOURCE_DIR}/hw")

set(RISCV_HW_LIBRARY_HEADERS
    "${_RV_HW_HDR_DIR}/hw_IDevice.h"
    "${_RV_HW_HDR_DIR}/hw_Scheduler.h"
)

set(RISCV_HW_LIBRARY_SOURCES
    "${_RV_HW_SRC_DIR}/hw_Scheduler.cpp"
)
