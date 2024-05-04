set(_RV_CPU_HDR_DIR "${RISCVLIB_HEADER_DIR}/RiscvEmu/cpu")
set(_RV_CPU_SRC_DIR "${RISCVLIB_SOURCE_DIR}/cpu")

set(RISCV_CPU_LIBRARY_HEADERS
    "${_RV_CPU_HDR_DIR}/cpu_EncodeInstruction.h"
    "${_RV_CPU_HDR_DIR}/cpu_Hart.h"
    "${_RV_CPU_HDR_DIR}/cpu_InstructionFormat.h"
    "${_RV_CPU_HDR_DIR}/cpu_Opcodes.h"
    "${_RV_CPU_HDR_DIR}/cpu_Result.h"
    "${_RV_CPU_HDR_DIR}/cpu_TrapCode.h"
    "${_RV_CPU_HDR_DIR}/cpu_Types.h"
    "${_RV_CPU_HDR_DIR}/cpu_Values.h"

    "${_RV_CPU_HDR_DIR}/detail/cpu_ClkTime.h"
    "${_RV_CPU_HDR_DIR}/detail/cpu_DecoderImpl.h"
    "${_RV_CPU_HDR_DIR}/detail/cpu_IntegerMultiply.h"
    "${_RV_CPU_HDR_DIR}/detail/cpu_MemoryManager.h"
)

set(RISCV_CPU_LIBRARY_SOURCES
    "${_RV_CPU_SRC_DIR}/cpu_Disassembler.cpp"

    "${_RV_CPU_SRC_DIR}/detail/cpu_ClkTime.cpp"
    "${_RV_CPU_SRC_DIR}/detail/cpu_IntegerMultiply.cpp"
    "${_RV_CPU_SRC_DIR}/detail/cpu_IntegerMultiplyImpl-arch.amd64.S"
    "${_RV_CPU_SRC_DIR}/detail/cpu_MemoryManager.cpp"

    "${_RV_CPU_SRC_DIR}/Hart/cpu_CsrReadWrite.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_Initialize.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_InstructionRunner.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_MemoryAccess.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_Reset.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_SharedState.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_Trap.cpp"
    "${_RV_CPU_SRC_DIR}/Hart/cpu_UserApi.cpp"
)
