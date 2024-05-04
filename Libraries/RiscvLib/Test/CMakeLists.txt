cmake_minimum_required(VERSION 3.20)

set(RISCV_TEST_COMMON_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")

project(RiscvEmuTests LANGUAGES C CXX)

# Set C++ Language.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Display compilier commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(RiscvEmuTestLib
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/cpu/test_HartTestCase.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/cpu/test_HartTestSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/test_Main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/test_TestFramework.cpp"
)

target_include_directories(RiscvEmuTestLib PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(RiscvEmuTestLib PUBLIC RiscvLib)

# Add tests.
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/RunTestPrograms")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuProfileMemoryMonitor")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestDecodingBType")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestDecodingIType")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestDecodingJType")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestDecodingRType")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestDecodingSType")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestDecodingUType")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeAUIPC")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeBRANCH")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeJAL")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeJALR")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeLOAD")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeLUI")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeOP")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeOP_32")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeOP_IMM")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeOP_IMM_32")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/CpuTestOpcodeSTORE")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/IntrptTestPLIC")
