#pragma once

#include "game.hpp"
#include <ncurses.h>

namespace tetris {

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void init();
    void render(const Game &game);
    void renderGameOver(const Game &game);
    void cleanup();

  private:
    WINDOW *game_win_;
    WINDOW *info_win_;

    void drawBorder(WINDOW *win, int height, int width);
    void drawBlock(WINDOW *win, int x, int y, int color);
};

} // namespace tetris
