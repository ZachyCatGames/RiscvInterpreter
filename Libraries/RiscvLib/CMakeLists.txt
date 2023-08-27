cmake_minimum_required(VERSION 3.20)

project(RiscvLib LANGUAGES C CXX)

# Set C++ Language.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Display compilier commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable additional compilier warnings.
add_compile_options(-Wall -Wextra)

# Disable C++ exceptions and stack protector.
add_compile_options(-fno-exceptions -fno-stack-protector)

# Check if we're using clang to add additional options.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Use lld as our linker and libc++ as our std library.
    add_link_options(-fuse-ld=lld -stdlib=libc++)
    add_compile_options(-stdlib=libc++)

    # Statically link libc++.
    # TODO: Remove when arch finally updates llvm.
    add_link_options(-static -lc++abi)

    # Enable full LTO on Release builds.
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_link_options(-flto=full)
    endif()
endif()

# Add our module directory.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)

# Include library modules.
include(UtilLibrarySources)

# Create our executable.
add_library(RiscvLib
    "${PROJECT_SOURCE_DIR}/Sources/Test.cpp"

    # Result library.
    ${RISCV_RESULT_LIBRARY_HEADERS}
    ${RISCV_RESULT_LIBRARY_SOURCES}

    # Util library.
    ${RISCV_UTIL_LIBRARY_HEADERS}
    ${RISCV_UTIL_LIBRARY_SOURCES}
)

# Add our Include directory the list of include directories.
target_include_directories(RiscvLib PUBLIC "${PROJECT_SOURCE_DIR}/Include")
