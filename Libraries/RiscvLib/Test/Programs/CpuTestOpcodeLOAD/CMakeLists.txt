add_executable(CpuTestOpcodeLOAD
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.Common.cpp"
)

target_include_directories(CpuTestOpcodeLOAD PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
target_link_libraries(CpuTestOpcodeLOAD PUBLIC RiscvLib RiscvEmuTestLib)

if(RISCV_CFG_CPU_ENABLE_RV64)
    add_executable(CpuTestOpcodeLOAD-For64
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.For64.cpp"
    )

    target_include_directories(CpuTestOpcodeLOAD-For64 PUBLIC ${RISCV_TEST_COMMON_HEADER_DIR})
    target_link_libraries(CpuTestOpcodeLOAD-For64 PUBLIC RiscvLib RiscvEmuTestLib)
endif()
