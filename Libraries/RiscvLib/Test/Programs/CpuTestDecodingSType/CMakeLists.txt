add_executable(CpuTestDecodingSType
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestDecodingSType PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestDecodingSType PUBLIC RiscvLib RiscvEmuTestLib)
