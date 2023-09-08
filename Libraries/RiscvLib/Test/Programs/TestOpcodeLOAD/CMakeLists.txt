cmake_minimum_required(VERSION 3.20)

project(TestOpcodeLOAD LANGUAGES C CXX)

add_executable(TestOpcodeLOAD
    "${PROJECT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeLOAD PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})

target_link_libraries(TestOpcodeLOAD PUBLIC RiscvLib RiscvEmuTestLib)