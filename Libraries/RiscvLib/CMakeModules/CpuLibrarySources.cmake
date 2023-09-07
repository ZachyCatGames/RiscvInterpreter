set(RISCV_CPU_LIBRARY_HEADERS
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/cpu_EncodeInstruction.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/cpu_Hart.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/cpu_PrivilageLevel.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/cpu_Result.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/decoder/cpu_DecoderImpl.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/decoder/cpu_InstructionFormat.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/decoder/cpu_Opcodes.h"
    "${PROJECT_SOURCE_DIR}/Include/RiscvEmu/cpu/decoder/cpu_Values.h"
)

set(RISCV_CPU_LIBRARY_SOURCES
    "${PROJECT_SOURCE_DIR}/Sources/cpu/Hart/cpu_Initialize.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/cpu/Hart/cpu_InstructionRunner.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/cpu/Hart/cpu_MemoryAccess.cpp"
    "${PROJECT_SOURCE_DIR}/Sources/cpu/Hart/cpu_Reset.cpp"
)