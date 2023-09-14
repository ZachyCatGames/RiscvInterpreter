cmake_minimum_required(VERSION 3.20)

project(RiscvLib LANGUAGES C CXX)

# Set C++ Language.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Display compilier commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable additional compilier warnings.
add_compile_options(-Wall -Wextra -Wconversion)

# Disable C++ exceptions and stack protector.
add_compile_options(-fno-exceptions -fno-stack-protector)

# Check if we're using clang to add additional options.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Use lld as our linker and libc++ as our std library.
    add_link_options(-fuse-ld=lld -stdlib=libc++ -fexperimental-library)
    add_compile_options(-stdlib=libc++ -fexperimental-library)

    # Enable full LTO on Release builds.
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_link_options(-flto=full)
        add_compile_options(-flto=full)
    endif()
endif()

# Add our module directory.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)

# Include library modules.
include(CpuLibrarySources)
include(HwLibrarySources)
include(MemLibrarySources)
include(ResultLibrarySources)
include(TestLibrarySources)
include(UtilLibrarySources)

# Create our executable.
add_library(RiscvLib
    "${PROJECT_SOURCE_DIR}/Sources/Test.cpp"

    # CPU library.
    ${RISCV_CPU_LIBRARY_HEADERS}
    ${RISCV_CPU_LIBRARY_SOURCES}

    # HW Library.
    ${RISCV_HW_LIBRARY_HEADERS}
    ${RISCV_HW_LIBRARY_SOURCES}

    # Memory library.
    ${RISCV_MEM_LIBRARY_HEADERS}
    ${RISCV_MEM_LIBRARY_SOURCES}

    # Result library.
    ${RISCV_RESULT_LIBRARY_HEADERS}
    ${RISCV_RESULT_LIBRARY_SOURCES}

    # Test library.
    ${RISCV_TEST_LIBRARY_HEADERS}
    ${RISCV_TEST_LIBRARY_SOURCES}

    # Util library.
    ${RISCV_UTIL_LIBRARY_HEADERS}
    ${RISCV_UTIL_LIBRARY_SOURCES}
)

# Enable RV64.
set(RISCV_CFG_CPU_ENABLE_RV64 true)

# Check if RV64 is enabled.
if (RISCV_CFG_CPU_ENABLE_RV64)
    target_compile_definitions(RiscvLib PUBLIC RISCV_CFG_CPU_ENABLE_RV64=${RISCV_CFG_CPU_ENABLE_RV64})
endif()

# Add our Include directory the list of include directories.
target_include_directories(RiscvLib PUBLIC "${PROJECT_SOURCE_DIR}/Include")

# Include tests.
if(NOT DEFINED ${RISCV_EMU_DISABLE_UNIT_TESTS})
    add_subdirectory(Test)
endif()
