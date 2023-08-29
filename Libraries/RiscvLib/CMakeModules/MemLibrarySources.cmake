set(RISCV_MEM_LIBRARY_HEADERS
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/detail/mem_IoRegion.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/detail/mem_MemRegion.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/detail/mem_RegionBase.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/mem_AlignedMmioDev.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/mem_IMmioDev.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/mem_MemoryController.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/mem_RegionInfo.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/mem/mem_Result.h"
)

set(RISCV_MEM_LIBRARY_SOURCES
    "${PROJECT_SOURCE_DIR}/Sources/mem/mem_MemoryController.cpp"
)