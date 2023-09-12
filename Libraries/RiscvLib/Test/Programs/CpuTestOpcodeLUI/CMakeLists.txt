add_executable(CpuTestOpcodeLUI
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeLUI PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeLUI PUBLIC RiscvLib RiscvEmuTestLib)
