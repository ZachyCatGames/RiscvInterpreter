add_executable(CpuTestOpcodeSTORE
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.Common.cpp"
)

target_include_directories(CpuTestOpcodeSTORE PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeSTORE PUBLIC RiscvLib RiscvEmuTestLib)

if(RISCV_CFG_CPU_ENABLE_RV64)
    add_executable(CpuTestOpcodeSTORE-For64
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.For64.cpp"
    )

    target_include_directories(CpuTestOpcodeSTORE-For64 PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
    target_link_libraries(CpuTestOpcodeSTORE-For64 PUBLIC RiscvLib RiscvEmuTestLib)
endif()
