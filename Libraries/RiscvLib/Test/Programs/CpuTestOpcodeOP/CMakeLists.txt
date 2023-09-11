add_executable(TestOpcodeOP
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeOP PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeOP PUBLIC RiscvLib RiscvEmuTestLib)
