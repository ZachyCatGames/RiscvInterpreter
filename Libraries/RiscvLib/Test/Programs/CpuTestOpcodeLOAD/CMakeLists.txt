add_executable(TestOpcodeLOAD
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeLOAD PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeLOAD PUBLIC RiscvLib RiscvEmuTestLib)
