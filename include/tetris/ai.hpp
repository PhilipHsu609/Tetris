#pragma once

#include "game.hpp"

namespace tetris {

class AI {
  public:
    AI();

    // Evaluate a position and return a score
    int evaluatePosition(const Board &board, const Tetromino &piece,
                         Position pos);

    // Find the best move for the current piece
    struct Move {
        int rotation;
        int x;
        int score;
    };

    Move findBestMove(const Game &game);

  private:
    int calculateHeight(const Board &board);
    int countHoles(const Board &board);
    int calculateBumpiness(const Board &board);
    int countCompletedLines(const Board &board);
};

} // namespace tetris
