#include <tetris/board.hpp>
#include <algorithm>

namespace tetris {

Board::Board() { reset(); }

void Board::reset() {
    for (auto &row : grid_) {
        row.fill(0);
    }
}

bool Board::canPlace(const Tetromino &piece, Position pos) const {
    for (const auto &block : piece.getBlocks()) {
        int x = pos.x + block.x;
        int y = pos.y + block.y;

        // Check boundaries
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
            return false;
        }

        // Check collision with existing blocks
        if (grid_[y][x] != 0) {
            return false;
        }
    }
    return true;
}

void Board::place(const Tetromino &piece, Position pos) {
    int color = static_cast<int>(piece.getType()) + 1;
    for (const auto &block : piece.getBlocks()) {
        int x = pos.x + block.x;
        int y = pos.y + block.y;
        if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
            grid_[y][x] = color;
        }
    }
}

int Board::clearLines() {
    int lines_cleared = 0;

    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool is_full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (grid_[y][x] == 0) {
                is_full = false;
                break;
            }
        }

        if (is_full) {
            lines_cleared++;
            // Move all lines above down
            for (int move_y = y; move_y > 0; move_y--) {
                grid_[move_y] = grid_[move_y - 1];
            }
            grid_[0].fill(0);
            y++; // Check this line again
        }
    }

    return lines_cleared;
}

bool Board::isGameOver() const {
    // Check if top row has any blocks
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (grid_[0][x] != 0) {
            return true;
        }
    }
    return false;
}

int Board::getCell(int x, int y) const {
    if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
        return grid_[y][x];
    }
    return 0;
}

} // namespace tetris
