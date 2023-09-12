add_executable(TestOpcodeBRANCH
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(TestOpcodeBRANCH PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(TestOpcodeBRANCH PUBLIC RiscvLib RiscvEmuTestLib)
