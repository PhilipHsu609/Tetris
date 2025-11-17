# Test executable target
set(TEST_TARGET mytest)

add_executable(${TEST_TARGET})

# Test source files and game source files for testing
target_sources(
    ${TEST_TARGET}
    PRIVATE
    test.cpp
    tetris_test.cpp
    ${PROJECT_SOURCE_DIR}/src/tetromino.cpp
    ${PROJECT_SOURCE_DIR}/src/board.cpp
    ${PROJECT_SOURCE_DIR}/src/game.cpp
    ${PROJECT_SOURCE_DIR}/src/ai.cpp
    ${PROJECT_SOURCE_DIR}/src/multiplayer.cpp
)

# Include directories for tests
target_include_directories(
    ${TEST_TARGET}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(
    ${TEST_TARGET}
    PRIVATE
    fmt::fmt                # fmt library for formatting
    GTest::gtest_main       # GoogleTest with main() provided
    project_compile_flags   # Custom compile flags
)

# C++ standard (inherits from root, but can be overridden here)
target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

# Automatically discover and register tests
include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})

