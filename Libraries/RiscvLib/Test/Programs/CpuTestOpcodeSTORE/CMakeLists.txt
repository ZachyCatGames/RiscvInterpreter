add_executable(TestOpcodeSTORE
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeSTORE PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeSTORE PUBLIC RiscvLib RiscvEmuTestLib)
