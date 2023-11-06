set(RISCV_DIAG_LIBRARY_HEADERS
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_Assert.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_DebugLog.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_Log.h"
)

set(RISCV_DIAG_LIBRARY_SOURCES
    "${PROJECT_SOURCE_DIR}/Sources/diag/detail/diag_AssertImpl.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/diag/detail/diag_DebugLogImpl.cpp"
)

