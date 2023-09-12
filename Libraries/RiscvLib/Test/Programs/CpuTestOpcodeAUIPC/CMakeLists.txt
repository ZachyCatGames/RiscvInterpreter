add_executable(CpuTestOpcodeAUIPC
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeAUIPC PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeAUIPC PUBLIC RiscvLib RiscvEmuTestLib)
