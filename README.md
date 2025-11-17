# Tetris

[![CI](https://github.com/PhilipHsu609/Tetris/actions/workflows/ci.yml/badge.svg)](https://github.com/PhilipHsu609/Tetris/actions/workflows/ci.yml)

A terminal-based Tetris game with AI auto-play functionality and multi-player mode, built with modern C++17.

## Features

- Classic Tetris gameplay in the terminal
- AI auto-play mode that can play the game automatically
- **Multi-player mode**: Watch multiple AI players compete simultaneously in the same terminal
- All 7 standard tetromino pieces (I, O, T, S, Z, J, L)
- Line clearing with scoring system
- Progressive difficulty levels
- Built with modern CMake and vcpkg for dependency management
- ncurses-based terminal UI

## Controls

### Single Player Mode (default or with argument `1`)
- **Arrow Keys**: Move piece left/right/down
- **Up Arrow**: Rotate piece
- **Space**: Hard drop
- **A**: Toggle AI auto-play mode
- **R**: Reset game
- **Q**: Quit game

### Multi-Player Mode (with argument `2` or more)
- **R**: Reset all games
- **Q**: Quit
- All players are controlled by AI automatically

## Quick Start

### Prerequisites

- CMake 3.15 or higher
- A C++17 compatible compiler (GCC or Clang)
- [vcpkg](https://github.com/microsoft/vcpkg) package manager
- ncurses library

### Setup

1. Install system dependencies:
   ```bash
   # On Ubuntu/Debian
   sudo apt-get install libncurses-dev
   ```

2. Set the `VCPKG_ROOT` environment variable to your vcpkg installation path:
   ```bash
   export VCPKG_ROOT=/path/to/vcpkg
   ```

### Building

Using the Makefile (Linux):
```bash
make all          # Configure, build, and test
make build        # Build only
make test         # Run tests
make clean        # Clean build artifacts
```

Or using CMake directly:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Running

**Single Player Mode (with AI or manual control):**
```bash
./build/src/tetris
# or explicitly
./build/src/tetris 1
```

The game will start in the terminal with AI auto-play enabled by default. Press 'A' to toggle between AI and manual control.

**Multi-Player Mode (watch multiple AIs compete):**
```bash
# 2 players
./build/src/tetris 2

# 3 players
./build/src/tetris 3

# Up to 8 players
./build/src/tetris 8
```

In multi-player mode, all players are AI-controlled and compete simultaneously. Watch their boards side-by-side in the terminal!

**Help:**
```bash
./build/src/tetris --help
```

## How the AI Works

The AI uses a heuristic-based evaluation function with optimized weights based on the research from [this article](https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/). The weights were determined through genetic algorithm optimization.

The evaluation function considers four key factors:

- **Aggregate Height** (weight: -0.510066): Sum of the heights of all columns - penalizes tall stacks
- **Complete Lines** (weight: +0.760666): Number of lines cleared - rewards line clearing
- **Holes** (weight: -0.35663): Empty cells with filled cells above them - heavily penalized as they're hard to fill
- **Bumpiness** (weight: -0.184483): Sum of absolute height differences between adjacent columns - prefers smooth surfaces

The AI evaluates all possible positions (rotations and horizontal placements) for each piece and selects the move with the highest score. This optimized approach allows the AI to play significantly better than simple heuristics.

## Project Structure

```
include/tetris/     # Public headers
├── tetromino.hpp   # Tetromino piece definitions
├── board.hpp       # Game board logic
├── game.hpp        # Game state management
├── renderer.hpp    # Terminal rendering with ncurses
├── ai.hpp          # AI decision-making
└── multiplayer.hpp # Multi-player game coordination

src/                # Implementation files
├── main.cpp        # Application entry point
├── tetromino.cpp
├── board.cpp
├── game.cpp
├── renderer.cpp
├── ai.cpp
└── multiplayer.cpp

test/               # Unit tests
└── test.cpp
```

## Development

### Adding Tests

This project uses GoogleTest. Add test files in the `test/` directory:

```cpp
#include <gtest/gtest.h>
#include <tetris/board.hpp>

TEST(BoardTest, CanPlacePiece) {
    tetris::Board board;
    tetris::Tetromino piece(tetris::TetrominoType::I);
    EXPECT_TRUE(board.canPlace(piece, {0, 0}));
}
```

### Code Style

- `.clang-format`: Code formatting rules (based on LLVM style)
- `.clang-tidy`: Static analysis rules

Run formatting:
```bash
clang-format -i src/*.cpp include/**/*.hpp
```

## Continuous Integration

The project includes a GitHub Actions workflow (`.github/workflows/ci.yml`) that:
- Builds the project on Linux
- Tests with both GCC and Clang compilers
- Runs all unit tests automatically
- Uploads test results on failure

The CI runs on every push and pull request to the `main` branch.

## License

This project is provided under the MIT License. See LICENSE file for details.

