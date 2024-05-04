set(_RV_MEM_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/mem")
set(_RV_MEM_SRC_DIR "${RISCVLIB_SOURCE_DIR}/mem")

set(RISCV_MEM_LIBRARY_HEADERS
    "${RISCVLIB_HEADER_DIR}/RiscvEmu/mem.h"

    "${_RV_MEM_HDR_DIR}/mem_AlignedMmioDev.h"
    "${_RV_MEM_HDR_DIR}/mem_IMmioDev.h"
    "${_RV_MEM_HDR_DIR}/mem_MCClient.h"
    "${_RV_MEM_HDR_DIR}/mem_MemoryController.h"
    "${_RV_MEM_HDR_DIR}/mem_RegionInfo.h"
    "${_RV_MEM_HDR_DIR}/mem_Result.h"

    "${_RV_MEM_HDR_DIR}/detail/mem_IoRegion.h"
    "${_RV_MEM_HDR_DIR}/detail/mem_MemRegion.h"
    "${_RV_MEM_HDR_DIR}/detail/mem_RegionBase.h"
)

set(RISCV_MEM_LIBRARY_SOURCES
    "${_RV_MEM_SRC_DIR}/mem_MemoryController.cpp"
    "${_RV_MEM_SRC_DIR}/mem_MemoryDevice.cpp"

    "${_RV_MEM_SRC_DIR}/detail/mem_MemoryDeviceImpl.cpp"
    "${_RV_MEM_SRC_DIR}/detail/mem_MemRegion.cpp"
)
