add_executable(TestOpcodeJAL
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeJAL PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeJAL PUBLIC RiscvLib RiscvEmuTestLib)
