add_executable(CpuTestOpcodeSTORE
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeSTORE PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeSTORE PUBLIC RiscvLib RiscvEmuTestLib)
