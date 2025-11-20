#pragma once

#include "ai.hpp"
#include "game.hpp"
#include <memory>
#include <vector>

namespace tetris {

class MultiPlayerGame {
  public:
    explicit MultiPlayerGame(int num_players);

    void update();
    void reset();

    int getNumPlayers() const { return num_players_; }
    const Game &getGame(int player_id) const { return *games_[player_id]; }
    bool isAnyPlaying() const;
    int getActivePlayers() const;

  private:
    int num_players_;
    std::vector<std::unique_ptr<Game>> games_;
    std::vector<std::unique_ptr<AI>> ais_;

    void makeAIMove(int player_id);
};

} // namespace tetris
