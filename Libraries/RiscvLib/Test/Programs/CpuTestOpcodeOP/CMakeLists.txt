add_executable(CpuTestOpcodeOP
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeOP PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeOP PUBLIC RiscvLib RiscvEmuTestLib)
