set(RISCV_DIAG_LIBRARY_HEADERS
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_Abort.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_Assert.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_DebugLog.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_FormatString.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/diag_Log.h"

    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/detail/diag_AbortImpl.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/detail/diag_AssertImpl.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/detail/diag_DebugLogImpl.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/diag/detail/diag_PrintSourceLocation.h"
)

set(RISCV_DIAG_LIBRARY_SOURCES
    "${PROJECT_SOURCE_DIR}/Sources/diag/diag_Abort.cpp"

    "${PROJECT_SOURCE_DIR}/Sources/diag/detail/diag_AbortImpl.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/diag/detail/diag_AssertImpl.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/diag/detail/diag_DebugLogImpl.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/diag/detail/diag_PrintSourceLocation.cpp"
)

