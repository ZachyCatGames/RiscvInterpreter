add_executable(CpuTestDecodingRType
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(CpuTestDecodingRType PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestDecodingRType PUBLIC RiscvLib RiscvEmuTestLib)
