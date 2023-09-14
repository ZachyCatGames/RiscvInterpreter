add_executable(CpuTestOpcodeOP_IMM_32-For64
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeOP_IMM_32-For64 PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeOP_IMM_32-For64 PUBLIC RiscvLib RiscvEmuTestLib)
