add_executable(CpuTestDecodingJType
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestDecodingJType PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestDecodingJType PUBLIC RiscvLib RiscvEmuTestLib)
