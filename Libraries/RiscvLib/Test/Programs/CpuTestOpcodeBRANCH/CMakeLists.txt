add_executable(CpuTestOpcodeBRANCH
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestOpcodeBRANCH PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeBRANCH PUBLIC RiscvLib RiscvEmuTestLib)
