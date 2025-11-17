#pragma once

#include "game.hpp"
#include "multiplayer.hpp"
#include <ncurses.h>
#include <vector>

namespace tetris {

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void init();
    void render(const Game &game);
    void renderGameOver(const Game &game);
    void renderMultiPlayer(const MultiPlayerGame &mp_game);
    void cleanup();

  private:
    WINDOW *game_win_;
    WINDOW *info_win_;
    int board_height_;
    int board_width_;
    std::vector<WINDOW *> mp_game_wins_;
    std::vector<WINDOW *> mp_info_wins_;

    void drawBorder(WINDOW *win, int height, int width);
    void drawBlock(WINDOW *win, int x, int y, int color);
    void renderSingleGame(WINDOW *game_win, WINDOW *info_win, const Game &game,
                          int player_id);
};

} // namespace tetris
