add_executable(TestOpcodeAUIPC
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeAUIPC PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeAUIPC PUBLIC RiscvLib RiscvEmuTestLib)
