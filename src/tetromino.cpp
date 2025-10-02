#include <tetris/tetromino.hpp>

namespace tetris {

Tetromino::Tetromino(TetrominoType type) : type_(type), rotation_(0) {
    updateBlocks();
}

void Tetromino::rotate() {
    rotation_ = (rotation_ + 1) % 4;
    updateBlocks();
}

void Tetromino::rotateBack() {
    rotation_ = (rotation_ + 3) % 4;
    updateBlocks();
}

void Tetromino::updateBlocks() { blocks_ = getShapeBlocks(rotation_); }

std::vector<Position> Tetromino::getShapeBlocks(int rotation) const {
    std::vector<Position> blocks;

    switch (type_) {
    case TetrominoType::I:
        if (rotation % 2 == 0) {
            blocks = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
        } else {
            blocks = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
        }
        break;

    case TetrominoType::O:
        blocks = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
        break;

    case TetrominoType::T:
        if (rotation == 0) {
            blocks = {{1, 0}, {0, 1}, {1, 1}, {2, 1}};
        } else if (rotation == 1) {
            blocks = {{1, 0}, {1, 1}, {2, 1}, {1, 2}};
        } else if (rotation == 2) {
            blocks = {{0, 1}, {1, 1}, {2, 1}, {1, 2}};
        } else {
            blocks = {{1, 0}, {0, 1}, {1, 1}, {1, 2}};
        }
        break;

    case TetrominoType::S:
        if (rotation % 2 == 0) {
            blocks = {{1, 0}, {2, 0}, {0, 1}, {1, 1}};
        } else {
            blocks = {{1, 0}, {1, 1}, {2, 1}, {2, 2}};
        }
        break;

    case TetrominoType::Z:
        if (rotation % 2 == 0) {
            blocks = {{0, 0}, {1, 0}, {1, 1}, {2, 1}};
        } else {
            blocks = {{2, 0}, {1, 1}, {2, 1}, {1, 2}};
        }
        break;

    case TetrominoType::J:
        if (rotation == 0) {
            blocks = {{0, 0}, {0, 1}, {1, 1}, {2, 1}};
        } else if (rotation == 1) {
            blocks = {{1, 0}, {2, 0}, {1, 1}, {1, 2}};
        } else if (rotation == 2) {
            blocks = {{0, 1}, {1, 1}, {2, 1}, {2, 2}};
        } else {
            blocks = {{1, 0}, {1, 1}, {0, 2}, {1, 2}};
        }
        break;

    case TetrominoType::L:
        if (rotation == 0) {
            blocks = {{2, 0}, {0, 1}, {1, 1}, {2, 1}};
        } else if (rotation == 1) {
            blocks = {{1, 0}, {1, 1}, {1, 2}, {2, 2}};
        } else if (rotation == 2) {
            blocks = {{0, 1}, {1, 1}, {2, 1}, {0, 2}};
        } else {
            blocks = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
        }
        break;
    }

    return blocks;
}

} // namespace tetris
