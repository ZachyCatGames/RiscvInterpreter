# Set C++ Language.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RunTestPrograms
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Main.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/CreateProcess.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sources/CreateProcess-os.linux.cpp"
)

set_target_properties(RunTestPrograms PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

get_property(TestVar TARGET RunTestPrograms PROPERTY RUNTIME_OUTPUT_DIRECTORY)

# Copy resources.
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources/" DESTINATION "${TestVar}/Resources")

target_link_libraries(RunTestPrograms PUBLIC RiscvLib RiscvEmuTestLib)
