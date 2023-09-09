cmake_minimum_required(VERSION 3.20)

project(RiscvInterpreter LANGUAGES C CXX)

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
    add_link_options(-fuse-ld=lld -stdlib=libc++)
    add_compile_options(-stdlib=libc++)

    # Enable full LTO on Release builds.
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_link_options(-flto=full)
        add_compile_options(-flto=full)
    endif()
endif()

# Create our executable.
add_executable(RiscvInterpreter
    "${PROJECT_SOURCE_DIR}/Sources/Main.cpp"
)

# Add our Include directory the list of include directories.
target_include_directories(RiscvInterpreter PRIVATE "${PROJECT_SOURCE_DIR}/Include")

# Add RiscvLib.
add_subdirectory("${PROJECT_SOURCE_DIR}/Libraries/RiscvLib")
target_link_libraries(RiscvInterpreter PRIVATE RiscvLib)