#pragma once

#include "tetromino.hpp"
#include <array>
#include <memory>
#include <vector>

namespace tetris {

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;

class Board {
  public:
    Board();

    bool canPlace(const Tetromino &piece, Position pos) const;
    void place(const Tetromino &piece, Position pos);
    int clearLines();
    bool isGameOver() const;
    int getCell(int x, int y) const;
    void reset();

    int getWidth() const { return BOARD_WIDTH; }
    int getHeight() const { return BOARD_HEIGHT; }

  private:
    std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> grid_;
};

} // namespace tetris
