set(RISCV_INTRPT_LIBRARY_HEADERS
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/intrpt/intrpt_PLIC.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/intrpt/intrpt_Result.h"
)

set(RISCV_INTRPT_LIBRARY_SOURCES
    "${PROJECT_SOURCE_DIR}/Sources/intrpt/intrpt_ITarget.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/intrpt/intrpt_PLIC.cpp"
)

