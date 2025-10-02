#pragma once

#include <array>
#include <vector>

namespace tetris {

enum class TetrominoType { I, O, T, S, Z, J, L };

struct Position {
    int x;
    int y;
};

class Tetromino {
  public:
    Tetromino(TetrominoType type);

    void rotate();
    void rotateBack();
    const std::vector<Position> &getBlocks() const { return blocks_; }
    TetrominoType getType() const { return type_; }
    int getRotation() const { return rotation_; }

  private:
    TetrominoType type_;
    int rotation_;
    std::vector<Position> blocks_;

    void updateBlocks();
    std::vector<Position> getShapeBlocks(int rotation) const;
};

} // namespace tetris
