add_executable(CpuTestOpcodeOP_IMM
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeOP_IMM PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeOP_IMM PUBLIC RiscvLib RiscvEmuTestLib)
