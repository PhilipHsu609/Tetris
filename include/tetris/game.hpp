#pragma once

#include "board.hpp"
#include "tetromino.hpp"
#include <memory>
#include <random>

namespace tetris {

enum class GameState { PLAYING, GAME_OVER };

class Game {
  public:
    Game();

    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();
    void drop();
    void update();
    void reset();

    const Board &getBoard() const { return board_; }
    const Tetromino &getCurrentPiece() const { return *current_piece_; }
    Position getCurrentPosition() const { return current_pos_; }
    int getScore() const { return score_; }
    int getLevel() const { return level_; }
    int getLinesCleared() const { return lines_cleared_; }
    GameState getState() const { return state_; }

  private:
    Board board_;
    std::unique_ptr<Tetromino> current_piece_;
    Position current_pos_;
    int score_;
    int level_;
    int lines_cleared_;
    GameState state_;
    std::mt19937 rng_;

    void spawnNewPiece();
    bool tryMove(int dx, int dy);
    void lockPiece();
};

} // namespace tetris
