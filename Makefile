BUILD_TYPE ?= Debug
BUILD_DIR = build
TEST_DIR = $(BUILD_DIR)/test

CMAKE_FLAGS=-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
            -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

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
