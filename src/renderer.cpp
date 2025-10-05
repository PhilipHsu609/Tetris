#include <tetris/renderer.hpp>

namespace tetris {

Renderer::Renderer() : game_win_(nullptr), info_win_(nullptr), board_height_(BOARD_HEIGHT), board_width_(BOARD_WIDTH) {}

Renderer::~Renderer() { cleanup(); }

void Renderer::init() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Initialize color pairs - using colors as background for compatibility
    init_pair(1, COLOR_BLACK, COLOR_CYAN);    // I
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);  // O
    init_pair(3, COLOR_BLACK, COLOR_MAGENTA); // T
    init_pair(4, COLOR_BLACK, COLOR_GREEN);   // S
    init_pair(5, COLOR_BLACK, COLOR_RED);     // Z
    init_pair(6, COLOR_BLACK, COLOR_BLUE);    // J
    init_pair(7, COLOR_BLACK, COLOR_WHITE);   // L

    // Get terminal size
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    // Calculate board dimensions based on terminal height
    // We need 2 lines for borders, 1 line for top margin, and some space for info
    // Minimum height is 10, maximum is BOARD_HEIGHT (20)
    int available_height = term_height - 3; // 1 for top margin, 2 for borders
    board_height_ = available_height < 10 ? 10 : (available_height > BOARD_HEIGHT ? BOARD_HEIGHT : available_height);
    board_width_ = BOARD_WIDTH;

    // Create windows with calculated dimensions
    int win_height = board_height_ + 2;
    int win_width = board_width_ * 2 + 2;
    game_win_ = newwin(win_height, win_width, 1, 2);
    info_win_ = newwin(win_height, 30, 1, win_width + 4);
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
        mvwaddstr(win, y + 1, x * 2 + 1, "  ");
        wattroff(win, COLOR_PAIR(color));
    }
}

void Renderer::render(const Game &game) {
    werase(game_win_);
    werase(info_win_);

    int win_height = board_height_ + 2;
    int win_width = board_width_ * 2 + 2;
    
    drawBorder(game_win_, win_height, win_width);
    drawBorder(info_win_, win_height, 30);

    // Draw board
    const Board &board = game.getBoard();
    // Only draw the visible portion of the board based on board_height_
    int start_y = board.getHeight() > board_height_ ? board.getHeight() - board_height_ : 0;
    for (int y = start_y; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            int cell = board.getCell(x, y);
            if (cell > 0) {
                drawBlock(game_win_, x, y - start_y, cell);
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
        if (y >= start_y && y < board.getHeight() && x >= 0 && x < board.getWidth()) {
            drawBlock(game_win_, x, y - start_y, color);
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
    int win_height = board_height_ + 2;
    int win_width = board_width_ * 2 + 2;
    drawBorder(game_win_, win_height, win_width);

    mvwprintw(game_win_, board_height_ / 2 - 1, board_width_ - 4, "GAME OVER");
    mvwprintw(game_win_, board_height_ / 2 + 1, board_width_ - 6,
              "Score: %d", game.getScore());
    mvwprintw(game_win_, board_height_ / 2 + 2, board_width_ - 8,
              "Press R to restart");

    wrefresh(game_win_);
}

} // namespace tetris
