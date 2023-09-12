add_executable(TestOpcodeJALR
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeJALR PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeJALR PUBLIC RiscvLib RiscvEmuTestLib)
