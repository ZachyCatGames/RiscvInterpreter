add_executable(CpuTestDecodingIType
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestDecodingIType PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestDecodingIType PUBLIC RiscvLib RiscvEmuTestLib)
