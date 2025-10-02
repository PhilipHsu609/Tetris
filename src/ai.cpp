#include <tetris/ai.hpp>
#include <limits>

namespace tetris {

AI::AI() {}

int AI::calculateHeight(const Board &board) {
    int total_height = 0;
    for (int x = 0; x < board.getWidth(); x++) {
        for (int y = 0; y < board.getHeight(); y++) {
            if (board.getCell(x, y) != 0) {
                total_height += board.getHeight() - y;
                break;
            }
        }
    }
    return total_height;
}

int AI::countHoles(const Board &board) {
    int holes = 0;
    for (int x = 0; x < board.getWidth(); x++) {
        bool found_block = false;
        for (int y = 0; y < board.getHeight(); y++) {
            if (board.getCell(x, y) != 0) {
                found_block = true;
            } else if (found_block) {
                holes++;
            }
        }
    }
    return holes;
}

int AI::calculateBumpiness(const Board &board) {
    std::vector<int> heights(board.getWidth(), 0);

    for (int x = 0; x < board.getWidth(); x++) {
        for (int y = 0; y < board.getHeight(); y++) {
            if (board.getCell(x, y) != 0) {
                heights[x] = board.getHeight() - y;
                break;
            }
        }
    }

    int bumpiness = 0;
    for (int x = 0; x < board.getWidth() - 1; x++) {
        bumpiness += std::abs(heights[x] - heights[x + 1]);
    }
    return bumpiness;
}

int AI::countCompletedLines(const Board &board) {
    int completed = 0;
    for (int y = 0; y < board.getHeight(); y++) {
        bool is_full = true;
        for (int x = 0; x < board.getWidth(); x++) {
            if (board.getCell(x, y) == 0) {
                is_full = false;
                break;
            }
        }
        if (is_full) {
            completed++;
        }
    }
    return completed;
}

int AI::evaluatePosition(const Board &board, const Tetromino &piece,
                         Position pos) {
    // Create a copy of the board to test
    Board test_board = board;

    // Check if we can place the piece
    if (!test_board.canPlace(piece, pos)) {
        return std::numeric_limits<int>::min();
    }

    // Place the piece
    test_board.place(piece, pos);
    int cleared_lines = test_board.clearLines();

    // Calculate heuristics
    int height = calculateHeight(test_board);
    int holes = countHoles(test_board);
    int bumpiness = calculateBumpiness(test_board);

    // Weights based on https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/
    // These weights were found through genetic algorithm optimization
    double score = 0.0;
    score += cleared_lines * 0.760666;   // Reward line clears
    score += height * (-0.510066);       // Penalize aggregate height
    score += holes * (-0.35663);         // Penalize holes
    score += bumpiness * (-0.184483);    // Penalize bumpiness

    // Scale to integer for comparison (multiply by 1000 to maintain precision)
    return static_cast<int>(score * 1000);
}

AI::Move AI::findBestMove(const Game &game) {
    const Board &board = game.getBoard();
    const Tetromino &piece = game.getCurrentPiece();

    Move best_move{0, 0, std::numeric_limits<int>::min()};

    // Try all rotations
    for (int rotation = 0; rotation < 4; rotation++) {
        Tetromino test_piece = piece;
        for (int r = 0; r < rotation; r++) {
            test_piece.rotate();
        }

        // Try all horizontal positions
        for (int x = -3; x < board.getWidth() + 3; x++) {
            // Drop to lowest position
            Position test_pos{x, 0};
            while (board.canPlace(test_piece, test_pos)) {
                test_pos.y++;
            }
            test_pos.y--; // Back up one

            if (test_pos.y >= 0) {
                int score = evaluatePosition(board, test_piece, test_pos);
                if (score > best_move.score) {
                    best_move.rotation = rotation;
                    best_move.x = x;
                    best_move.score = score;
                }
            }
        }
    }

    return best_move;
}

} // namespace tetris
