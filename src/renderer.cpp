#include <tetris/renderer.hpp>

#include <algorithm>

namespace tetris {

// Minimum dimensions for a reasonable display
constexpr int MIN_BOARD_HEIGHT = 10;
constexpr int INFO_PANEL_WIDTH = 20;
// Row height includes: board height + 2 (borders) + 1 (spacing between rows)
constexpr int ROW_HEIGHT_OVERHEAD = 3;

Renderer::Renderer()
    : game_win_(nullptr), info_win_(nullptr), board_height_(BOARD_HEIGHT),
      board_width_(BOARD_WIDTH), term_height_(0), term_width_(0),
      last_num_players_(0), cached_layout_{0, 0, 0} {}

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

    // Initialize terminal size tracking
    updateTerminalSize();

    // Create windows with calculated dimensions
    int win_height = board_height_ + 2;
    int win_width = board_width_ * 2 + 2;
    game_win_ = newwin(win_height, win_width, 1, 2);
    info_win_ = newwin(win_height, 30, 1, win_width + 4);
}

void Renderer::updateTerminalSize() {
    int new_height, new_width;
    getmaxyx(stdscr, new_height, new_width);

    // Check if terminal size changed
    if (new_height != term_height_ || new_width != term_width_) {
        term_height_ = new_height;
        term_width_ = new_width;

        // Calculate board dimensions based on terminal height
        // We need 2 lines for borders, 1 line for top margin, and some space
        // for info
        int available_height = term_height_ - 3; // 1 for top margin, 2 for borders
        board_height_ = std::clamp(available_height, MIN_BOARD_HEIGHT, BOARD_HEIGHT);
        board_width_ = BOARD_WIDTH;
    }
}

void Renderer::cleanup() {
    if (game_win_)
        delwin(game_win_);
    if (info_win_)
        delwin(info_win_);
    for (auto win : mp_game_wins_) {
        if (win)
            delwin(win);
    }
    for (auto win : mp_info_wins_) {
        if (win)
            delwin(win);
    }
    mp_game_wins_.clear();
    mp_info_wins_.clear();
    endwin();
}

void Renderer::drawBorder(WINDOW *win, int height, int width) {
    // Parameters are available for future enhancements but not currently used
    (void)height;
    (void)width;
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

void Renderer::renderSingleGame(WINDOW *game_win, WINDOW *info_win,
                                 const Game &game, int player_id) {
    werase(game_win);
    werase(info_win);

    int win_height = board_height_ + 2;
    int win_width = board_width_ * 2 + 2;

    drawBorder(game_win, win_height, win_width);
    drawBorder(info_win, win_height, INFO_PANEL_WIDTH);

    const Board &board = game.getBoard();
    int start_y =
        board.getHeight() > board_height_ ? board.getHeight() - board_height_ : 0;

    // Draw board
    for (int y = start_y; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            int cell = board.getCell(x, y);
            if (cell > 0) {
                drawBlock(game_win, x, y - start_y, cell);
            }
        }
    }

    // Draw current piece if game is still playing
    if (game.getState() == GameState::PLAYING) {
        const Tetromino &piece = game.getCurrentPiece();
        Position pos = game.getCurrentPosition();
        int color = static_cast<int>(piece.getType()) + 1;
        for (const auto &block : piece.getBlocks()) {
            int x = pos.x + block.x;
            int y = pos.y + block.y;
            if (y >= start_y && y < board.getHeight() && x >= 0 &&
                x < board.getWidth()) {
                drawBlock(game_win, x, y - start_y, color);
            }
        }
    }

    // Draw info
    mvwprintw(info_win, 1, 1, "Player %d", player_id + 1);
    mvwprintw(info_win, 3, 1, "Score: %d", game.getScore());
    mvwprintw(info_win, 4, 1, "Level: %d", game.getLevel());
    mvwprintw(info_win, 5, 1, "Lines: %d", game.getLinesCleared());

    if (game.getState() == GameState::GAME_OVER) {
        mvwprintw(info_win, 7, 1, "GAME OVER");
    }

    wrefresh(game_win);
    wrefresh(info_win);
}

LayoutInfo Renderer::calculateLayout(int num_players) const {
    LayoutInfo layout;

    // Calculate how many players can fit per row
    // Each player needs: board_width * 2 + 2 (game) + INFO_PANEL_WIDTH (info) +
    // 2 (spacing)
    int player_width = board_width_ * 2 + 2 + INFO_PANEL_WIDTH + 2;

    // Leave some margin on the sides
    int available_width = term_width_ - 4;
    layout.cols = std::max(1, available_width / player_width);

    // Don't use more columns than players
    layout.cols = std::min(layout.cols, num_players);

    // Calculate rows needed
    layout.rows = (num_players + layout.cols - 1) / layout.cols;

    // Calculate the height available for each row
    // Reserve: 1 for top margin, 2 for control info at bottom
    int available_height = term_height_ - ROW_HEIGHT_OVERHEAD;
    int height_per_row = available_height / layout.rows;

    // Each player needs: board height + ROW_HEIGHT_OVERHEAD (2 borders + 1 row spacing)
    layout.player_board_height = height_per_row - ROW_HEIGHT_OVERHEAD;
    layout.player_board_height =
        std::clamp(layout.player_board_height, MIN_BOARD_HEIGHT, BOARD_HEIGHT);

    return layout;
}

void Renderer::recreateMultiPlayerWindows(int num_players) {
    // Clear old windows
    for (auto win : mp_game_wins_) {
        if (win)
            delwin(win);
    }
    for (auto win : mp_info_wins_) {
        if (win)
            delwin(win);
    }
    mp_game_wins_.clear();
    mp_info_wins_.clear();

    // Calculate and cache layout
    cached_layout_ = calculateLayout(num_players);

    // Update board height for rendering
    board_height_ = cached_layout_.player_board_height;

    int win_height = board_height_ + 2;
    int win_width = board_width_ * 2 + 2;
    int player_width = win_width + INFO_PANEL_WIDTH + 2;
    int row_height = win_height + 1;

    // Create windows for each player
    // Note: Windows that don't fit on screen are set to nullptr and skipped during rendering
    for (int i = 0; i < num_players; i++) {
        int col = i % cached_layout_.cols;
        int row = i / cached_layout_.cols;

        int x_offset = 2 + col * player_width;
        int y_offset = 1 + row * row_height;

        // Check if window would be visible
        if (x_offset + win_width <= term_width_ && y_offset + win_height <= term_height_) {
            mp_game_wins_.push_back(newwin(win_height, win_width, y_offset, x_offset));
            mp_info_wins_.push_back(
                newwin(win_height, INFO_PANEL_WIDTH, y_offset, x_offset + win_width + 2));
        } else {
            // Window doesn't fit - mark as nullptr to skip during rendering
            mp_game_wins_.push_back(nullptr);
            mp_info_wins_.push_back(nullptr);
        }
    }

    last_num_players_ = num_players;
}

void Renderer::renderMultiPlayer(const MultiPlayerGame &mp_game) {
    int num_players = mp_game.getNumPlayers();

    // Check for terminal resize
    int new_height, new_width;
    getmaxyx(stdscr, new_height, new_width);
    bool terminal_resized = (new_height != term_height_ || new_width != term_width_);

    if (terminal_resized) {
        // Clear the screen on resize
        clear();
        updateTerminalSize();
    }

    // Check if we need to recreate windows
    bool needs_recreate = (mp_game_wins_.size() != static_cast<size_t>(num_players)) ||
                          (last_num_players_ != num_players) ||
                          terminal_resized;

    if (needs_recreate) {
        recreateMultiPlayerWindows(num_players);
    }

    // Render each player's game
    for (int i = 0; i < num_players; i++) {
        size_t idx = static_cast<size_t>(i);
        if (mp_game_wins_[idx] != nullptr && mp_info_wins_[idx] != nullptr) {
            renderSingleGame(mp_game_wins_[idx], mp_info_wins_[idx],
                             mp_game.getGame(i), i);
        }
    }

    // Use cached layout for status bar position
    int row_height = cached_layout_.player_board_height + ROW_HEIGHT_OVERHEAD;

    // Draw control info at the bottom
    int info_y = 1 + cached_layout_.rows * row_height;
    if (info_y < term_height_ - 1) {
        // Clear the info line area first
        move(info_y, 0);
        clrtoeol();
        move(info_y + 1, 0);
        clrtoeol();

        mvprintw(info_y, 2, "Controls: R - Reset | Q - Quit | Terminal: %dx%d",
                 term_width_, term_height_);
        if (!mp_game.isAnyPlaying()) {
            mvprintw(info_y + 1, 2, "All games finished! Press R to restart.");
        } else {
            mvprintw(info_y + 1, 2, "Active players: %d/%d  Layout: %dx%d",
                     mp_game.getActivePlayers(), num_players,
                     cached_layout_.cols, cached_layout_.rows);
        }
    }

    refresh();
}

} // namespace tetris
