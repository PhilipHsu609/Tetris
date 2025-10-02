#include <tetris/renderer.hpp>

namespace tetris {

Renderer::Renderer() : game_win_(nullptr), info_win_(nullptr) {}

Renderer::~Renderer() { cleanup(); }

void Renderer::init() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Initialize color pairs
    init_pair(1, COLOR_CYAN, COLOR_BLACK);    // I
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // O
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // T
    init_pair(4, COLOR_GREEN, COLOR_BLACK);   // S
    init_pair(5, COLOR_RED, COLOR_BLACK);     // Z
    init_pair(6, COLOR_BLUE, COLOR_BLACK);    // J
    init_pair(7, COLOR_WHITE, COLOR_BLACK);   // L

    // Create windows
    int board_width = BOARD_WIDTH * 2 + 2;
    int board_height = BOARD_HEIGHT + 2;
    game_win_ = newwin(board_height, board_width, 1, 2);
    info_win_ = newwin(board_height, 30, 1, board_width + 4);
}

void Renderer::cleanup() {
    if (game_win_)
        delwin(game_win_);
    if (info_win_)
        delwin(info_win_);
    endwin();
}

void Renderer::drawBorder(WINDOW *win, int height, int width) {
    box(win, 0, 0);
}

void Renderer::drawBlock(WINDOW *win, int x, int y, int color) {
    if (color > 0) {
        wattron(win, COLOR_PAIR(color));
        mvwaddstr(win, y + 1, x * 2 + 1, "[]");
        wattroff(win, COLOR_PAIR(color));
    }
}

void Renderer::render(const Game &game) {
    werase(game_win_);
    werase(info_win_);

    drawBorder(game_win_, BOARD_HEIGHT + 2, BOARD_WIDTH * 2 + 2);
    drawBorder(info_win_, BOARD_HEIGHT + 2, 30);

    // Draw board
    const Board &board = game.getBoard();
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            int cell = board.getCell(x, y);
            if (cell > 0) {
                drawBlock(game_win_, x, y, cell);
            }
        }
    }

    // Draw current piece
    const Tetromino &piece = game.getCurrentPiece();
    Position pos = game.getCurrentPosition();
    int color = static_cast<int>(piece.getType()) + 1;
    for (const auto &block : piece.getBlocks()) {
        int x = pos.x + block.x;
        int y = pos.y + block.y;
        if (y >= 0 && y < board.getHeight() && x >= 0 && x < board.getWidth()) {
            drawBlock(game_win_, x, y, color);
        }
    }

    // Draw info
    mvwprintw(info_win_, 2, 2, "TETRIS with AI");
    mvwprintw(info_win_, 4, 2, "Score: %d", game.getScore());
    mvwprintw(info_win_, 5, 2, "Level: %d", game.getLevel());
    mvwprintw(info_win_, 6, 2, "Lines: %d", game.getLinesCleared());
    mvwprintw(info_win_, 8, 2, "Controls:");
    mvwprintw(info_win_, 9, 2, "  A - Auto play");
    mvwprintw(info_win_, 10, 2, "  Arrow keys - Move");
    mvwprintw(info_win_, 11, 2, "  Up - Rotate");
    mvwprintw(info_win_, 12, 2, "  Space - Drop");
    mvwprintw(info_win_, 13, 2, "  R - Reset");
    mvwprintw(info_win_, 14, 2, "  Q - Quit");

    wrefresh(game_win_);
    wrefresh(info_win_);
    refresh();
}

void Renderer::renderGameOver(const Game &game) {
    werase(game_win_);
    drawBorder(game_win_, BOARD_HEIGHT + 2, BOARD_WIDTH * 2 + 2);

    mvwprintw(game_win_, BOARD_HEIGHT / 2 - 1, BOARD_WIDTH - 4, "GAME OVER");
    mvwprintw(game_win_, BOARD_HEIGHT / 2 + 1, BOARD_WIDTH - 6,
              "Score: %d", game.getScore());
    mvwprintw(game_win_, BOARD_HEIGHT / 2 + 2, BOARD_WIDTH - 8,
              "Press R to restart");

    wrefresh(game_win_);
}

} // namespace tetris
