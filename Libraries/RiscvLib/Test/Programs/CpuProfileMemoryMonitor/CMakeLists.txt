add_executable(CpuProfileMemoryMonitor
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuProfileMemoryMonitor PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuProfileMemoryMonitor PUBLIC RiscvLib RiscvEmuTestLib)
