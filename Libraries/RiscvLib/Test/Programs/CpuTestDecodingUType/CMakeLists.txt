add_executable(CpuTestDecodingUType
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestDecodingUType PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestDecodingUType PUBLIC RiscvLib RiscvEmuTestLib)
