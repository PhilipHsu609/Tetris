# Using This Template

This document explains how to use this template to start your own C++ project.

## Getting Started

### 1. Create Your Project from Template

On GitHub:
- Click "Use this template" button
- Choose a name for your new project
- Clone your new repository

Or manually:
```bash
git clone https://github.com/PhilipHsu609/CppTemplate.git MyNewProject
cd MyNewProject
rm -rf .git
git init
```

### 2. Customize the Project

1. **Update Project Name**: 
   - In `CMakeLists.txt`, change `MyProject` to your project name
   - In `vcpkg.json`, change `myproject` to your project name (lowercase)

2. **Update Include Path**:
   - Rename `include/project/` to `include/<your-project-name>/`
   - Update includes in source files accordingly

3. **Update README.md**:
   - Replace template content with your project description
   - Add project-specific build instructions

4. **Update LICENSE**:
   - Replace copyright holder with your name/organization
   - Change license type if needed

### 3. Add Your Code

1. **Add Headers**: Place public headers in `include/<project-name>/`
2. **Add Sources**: Place implementation files in `src/`
3. **Add Tests**: Add test files in `test/` with `_test.cpp` suffix
4. **Update CMakeLists.txt**: Add new source files to the appropriate CMakeLists.txt

### 4. Manage Dependencies

To add a new library:

1. Check if it's available in vcpkg: `vcpkg search <library-name>`
2. Add it to `vcpkg.json`:
   ```json
   {
     "dependencies": [
       "fmt",
       "gtest",
       "your-new-library"
     ]
   }
   ```
3. Add `find_package()` in root `CMakeLists.txt`
4. Link it in your target's `CMakeLists.txt`

**Note on vcpkg-configuration.json**: This file pins the vcpkg baseline to ensure reproducible builds. To update to newer package versions:
1. Visit https://github.com/microsoft/vcpkg/commits/master
2. Copy the latest commit SHA
3. Update the `baseline` field in `vcpkg-configuration.json`

### 5. Code Style

This template comes with pre-configured code style tools:

**Format code**:
```bash
# Format a single file
clang-format -i src/myfile.cpp

# Format all files
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

**Check code quality**:
```bash
# Run clang-tidy on a file
clang-tidy src/myfile.cpp -- -I./include

# Or use CMake compile_commands.json
cmake -S . -B build
clang-tidy -p build src/myfile.cpp
```

### 6. Development Workflow

```bash
# 1. Configure build (first time or after CMake changes)
make config

# 2. Build
make build

# 3. Run tests
make test

# 4. Clean build
make clean
```

Or use CMake directly:
```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build

# Test
cd build && ctest --output-on-failure

# Clean
rm -rf build
```

## Project Structure Best Practices

### Header Organization

```
include/
└── myproject/           # Project namespace directory
    ├── core.hpp         # Core functionality
    ├── utils.hpp        # Utility functions
    └── detail/          # Internal implementation details
        └── impl.hpp
```

### Source Organization

```
src/
├── CMakeLists.txt
├── main.cpp             # Application entry point
├── core.cpp             # Implementation of core.hpp
└── utils.cpp            # Implementation of utils.hpp
```

### Test Organization

```
test/
├── CMakeLists.txt
├── core_test.cpp        # Tests for core functionality
└── utils_test.cpp       # Tests for utilities
```

## Tips

1. **Keep it Simple**: This template is designed for small projects. Don't over-engineer.

2. **One Class Per File**: Generally, keep one class per .cpp/.hpp pair for easier navigation.

3. **Separate Concerns**: Keep public API in `include/`, implementation in `src/`.

4. **Write Tests Early**: Add tests as you add features, not after.

5. **Use Modern C++**: Take advantage of C++17 features like structured bindings, if constexpr, etc.

6. **Minimal Dependencies**: Only add dependencies you actually need. The template starts with just fmt and gtest.

## Common Tasks

### Adding a New Library/Module

1. Create header in `include/<project>/mylib.hpp`
2. Create implementation in `src/mylib.cpp`
3. Add to `src/CMakeLists.txt`:
   ```cmake
   target_sources(
       ${EXECUTABLE_NAME}
       PRIVATE
       main.cpp
       myclass.cpp
       mylib.cpp  # <-- Add this
   )
   ```
4. Create tests in `test/mylib_test.cpp`
5. Add to `test/CMakeLists.txt` if needed

### Creating Multiple Executables

If you need multiple executables:

```cmake
# In src/CMakeLists.txt
add_executable(tool1 tool1.cpp shared.cpp)
add_executable(tool2 tool2.cpp shared.cpp)

target_link_libraries(tool1 PRIVATE project_compile_flags fmt::fmt)
target_link_libraries(tool2 PRIVATE project_compile_flags fmt::fmt)
```

### Adding Examples

1. Create `examples/` directory
2. Add `examples/CMakeLists.txt`:
   ```cmake
   include(${PROJECT_SOURCE_DIR}/cmake/compile_all_files.cmake)
   ```
3. Add to root `CMakeLists.txt`:
   ```cmake
   add_subdirectory(examples)
   ```

## Troubleshooting

**vcpkg not found**:
- Ensure `VCPKG_ROOT` environment variable is set
- Or comment out vcpkg toolchain in root `CMakeLists.txt`

**Compile errors with warnings**:
- The template treats warnings as errors on Linux
- Fix the warnings or adjust `cmake/CompileFlags.cmake`

**Tests not discovered**:
- Ensure test functions use `TEST()` macro
- Check that `gtest_discover_tests()` is called in test CMakeLists.txt

## Need Help?

- Check the [CMake documentation](https://cmake.org/documentation/)
- Check the [vcpkg documentation](https://vcpkg.io/)
- Review the example code in this template
