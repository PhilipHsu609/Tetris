# Test executable target
set(TEST_TARGET mytest)

add_executable(${TEST_TARGET})

# Test source files
target_sources(
    ${TEST_TARGET}
    PRIVATE
    test.cpp
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

