add_executable(CpuTestOpcodeLOAD
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeLOAD PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeLOAD PUBLIC RiscvLib RiscvEmuTestLib)
