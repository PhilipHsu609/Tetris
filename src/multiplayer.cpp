#include <tetris/multiplayer.hpp>

namespace tetris {

MultiPlayerGame::MultiPlayerGame(int num_players) : num_players_(num_players) {
    for (int i = 0; i < num_players_; i++) {
        games_.push_back(std::make_unique<Game>());
        ais_.push_back(std::make_unique<AI>());
    }
}

void MultiPlayerGame::update() {
    for (int i = 0; i < num_players_; i++) {
        if (games_[i]->getState() == GameState::PLAYING) {
            makeAIMove(i);
        }
    }
}

void MultiPlayerGame::reset() {
    for (auto &game : games_) {
        game->reset();
    }
}

bool MultiPlayerGame::isAnyPlaying() const {
    for (const auto &game : games_) {
        if (game->getState() == GameState::PLAYING) {
            return true;
        }
    }
    return false;
}

int MultiPlayerGame::getActivePlayers() const {
    int active = 0;
    for (const auto &game : games_) {
        if (game->getState() == GameState::PLAYING) {
            active++;
        }
    }
    return active;
}

void MultiPlayerGame::makeAIMove(int player_id) {
    Game &game = *games_[player_id];
    AI &ai = *ais_[player_id];

    AI::Move move = ai.findBestMove(game);

    // Apply rotations
    for (int i = 0; i < move.rotation; i++) {
        game.rotate();
    }

    // Move to target x position
    Position current_pos = game.getCurrentPosition();
    while (current_pos.x < move.x) {
        game.moveRight();
        current_pos = game.getCurrentPosition();
    }
    while (current_pos.x > move.x) {
        game.moveLeft();
        current_pos = game.getCurrentPosition();
    }

    // Drop the piece
    game.drop();
}

} // namespace tetris
