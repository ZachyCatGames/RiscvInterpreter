add_executable(CpuTestOpcodeJALR
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeJALR PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeJALR PUBLIC RiscvLib RiscvEmuTestLib)
