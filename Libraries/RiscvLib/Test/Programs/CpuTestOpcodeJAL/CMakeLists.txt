add_executable(CpuTestOpcodeJAL
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeJAL PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeJAL PUBLIC RiscvLib RiscvEmuTestLib)
