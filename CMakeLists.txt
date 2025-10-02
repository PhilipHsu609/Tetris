cmake_minimum_required(VERSION 3.15)

# Include CMake utilities
include(cmake/debug.cmake)
include(cmake/CompileFlags.cmake)

# Optional: Set vcpkg toolchain file if VCPKG_ROOT is defined
# This allows building without vcpkg by commenting out or not setting VCPKG_ROOT
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE 
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Project definition
# TODO: Change 'MyProject' to your project name
project(
    MyProject
    VERSION 1.0.0
    DESCRIPTION "C++ Project Template"
    LANGUAGES CXX
)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd, clang-tidy, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies (managed via vcpkg.json)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Add source directory
add_subdirectory(src)

# Enable testing and add test directory
enable_testing()
add_subdirectory(test)

