add_executable(CpuTestRTypeDecoding
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestRTypeDecoding PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestRTypeDecoding PUBLIC RiscvLib RiscvEmuTestLib)
