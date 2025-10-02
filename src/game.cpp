#include <tetris/game.hpp>
#include <chrono>

namespace tetris {

Game::Game()
    : score_(0), level_(1), lines_cleared_(0), state_(GameState::PLAYING) {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng_.seed(seed);
    spawnNewPiece();
}

void Game::reset() {
    board_.reset();
    score_ = 0;
    level_ = 1;
    lines_cleared_ = 0;
    state_ = GameState::PLAYING;
    spawnNewPiece();
}

void Game::spawnNewPiece() {
    std::uniform_int_distribution<int> dist(0, 6);
    TetrominoType type = static_cast<TetrominoType>(dist(rng_));
    current_piece_ = std::make_unique<Tetromino>(type);
    current_pos_ = {BOARD_WIDTH / 2 - 1, 0};

    if (!board_.canPlace(*current_piece_, current_pos_)) {
        state_ = GameState::GAME_OVER;
    }
}

void Game::moveLeft() {
    if (state_ != GameState::PLAYING)
        return;
    tryMove(-1, 0);
}

void Game::moveRight() {
    if (state_ != GameState::PLAYING)
        return;
    tryMove(1, 0);
}

void Game::moveDown() {
    if (state_ != GameState::PLAYING)
        return;
    if (!tryMove(0, 1)) {
        lockPiece();
    }
}

void Game::rotate() {
    if (state_ != GameState::PLAYING)
        return;
    current_piece_->rotate();
    if (!board_.canPlace(*current_piece_, current_pos_)) {
        // Try wall kick
        if (!tryMove(-1, 0) && !tryMove(1, 0)) {
            current_piece_->rotateBack();
        }
    }
}

void Game::drop() {
    if (state_ != GameState::PLAYING)
        return;
    while (tryMove(0, 1)) {
    }
    lockPiece();
}

void Game::update() {
    if (state_ != GameState::PLAYING)
        return;
    moveDown();
}

bool Game::tryMove(int dx, int dy) {
    Position new_pos = {current_pos_.x + dx, current_pos_.y + dy};
    if (board_.canPlace(*current_piece_, new_pos)) {
        current_pos_ = new_pos;
        return true;
    }
    return false;
}

void Game::lockPiece() {
    board_.place(*current_piece_, current_pos_);
    int cleared = board_.clearLines();

    if (cleared > 0) {
        lines_cleared_ += cleared;
        // Score: 100 for 1 line, 300 for 2, 500 for 3, 800 for 4
        int points[] = {0, 100, 300, 500, 800};
        score_ += points[cleared] * level_;
        level_ = 1 + lines_cleared_ / 10;
    }

    if (board_.isGameOver()) {
        state_ = GameState::GAME_OVER;
    } else {
        spawnNewPiece();
    }
}

} // namespace tetris
