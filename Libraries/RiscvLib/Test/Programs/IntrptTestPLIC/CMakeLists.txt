add_executable(IntrptTestPLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"
)

target_include_directories(IntrptTestPLIC PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(IntrptTestPLIC PUBLIC RiscvLib RiscvEmuTestLib)
