BUILD_TYPE ?= Debug
BUILD_DIR = build
TEST_DIR = $(BUILD_DIR)/test

# Detect vcpkg toolchain file if VCPKG_ROOT is set
ifdef VCPKG_ROOT
	VCPKG_TOOLCHAIN_FILE = $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake
else
	# Try to find vcpkg in common locations
	ifneq ($(wildcard /usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake),)
		VCPKG_TOOLCHAIN_FILE = /usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake
	endif
endif

CMAKE_FLAGS=-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

# Add vcpkg toolchain file if found
ifdef VCPKG_TOOLCHAIN_FILE
	CMAKE_FLAGS += -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN_FILE)
endif

.PHONY: all config build clean test

all: config build test

config:
	cmake $(CMAKE_FLAGS) -S . -B $(BUILD_DIR)

build: config
	cmake --build $(BUILD_DIR)

test: build
	cd $(TEST_DIR) && ctest

clean:
	rm -rf $(BUILD_DIR)
