# Main executable target
# TODO: Change 'main' to your preferred executable name
set(EXECUTABLE_NAME main)

add_executable(${EXECUTABLE_NAME})

# Source files
target_sources(
    ${EXECUTABLE_NAME}
    PRIVATE
    main.cpp
    myclass.cpp
)

# Include directories
target_include_directories(
    ${EXECUTABLE_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(
    ${EXECUTABLE_NAME}
    PRIVATE
    project_compile_flags  # Custom compile flags from cmake/CompileFlags.cmake
    fmt::fmt               # fmt library for formatting
)

# C++ standard (inherits from root, but can be overridden here)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)

