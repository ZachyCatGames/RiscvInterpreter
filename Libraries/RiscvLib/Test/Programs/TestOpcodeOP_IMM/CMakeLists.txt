add_executable(TestOpcodeOP_IMM
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeOP_IMM PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeOP_IMM PUBLIC RiscvLib RiscvEmuTestLib)
