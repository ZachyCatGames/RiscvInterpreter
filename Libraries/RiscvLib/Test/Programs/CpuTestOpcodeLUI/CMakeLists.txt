add_executable(TestOpcodeLUI
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeLUI PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeLUI PUBLIC RiscvLib RiscvEmuTestLib)
