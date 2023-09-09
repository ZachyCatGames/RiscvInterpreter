cmake_minimum_required(VERSION 3.20)

set(RISCV_TEST_COMMON_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")

project(RiscvEmuTests LANGUAGES C CXX)

# Set C++ Language.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Display compilier commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(RiscvEmuTestLib
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/test_HartTestSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/test_Main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/test_TestFramework.cpp"
)

target_include_directories(RiscvEmuTestLib PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(RiscvEmuTestLib PUBLIC RiscvLib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/TestOpcodeLOAD")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/TestOpcodeOP_IMM")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Programs/TestOpcodeSTORE")