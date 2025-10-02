#include <tetris/ai.hpp>
#include <tetris/game.hpp>
#include <tetris/renderer.hpp>

#include <chrono>
#include <thread>

int main() {
    tetris::Game game;
    tetris::Renderer renderer;
    tetris::AI ai;

    renderer.init();

    bool running = true;
    bool auto_play = false;
    auto last_update = std::chrono::steady_clock::now();
    int update_interval = 500; // ms

    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                   last_update)
                .count();

        // Handle input
        int ch = getch();
        switch (ch) {
        case 'q':
        case 'Q':
            running = false;
            break;
        case 'r':
        case 'R':
            game.reset();
            auto_play = false;
            break;
        case 'a':
        case 'A':
            auto_play = !auto_play;
            break;
        case KEY_LEFT:
            if (!auto_play)
                game.moveLeft();
            break;
        case KEY_RIGHT:
            if (!auto_play)
                game.moveRight();
            break;
        case KEY_DOWN:
            if (!auto_play)
                game.moveDown();
            break;
        case KEY_UP:
            if (!auto_play)
                game.rotate();
            break;
        case ' ':
            if (!auto_play)
                game.drop();
            break;
        }

        // Auto play mode
        if (auto_play && game.getState() == tetris::GameState::PLAYING) {
            tetris::AI::Move move = ai.findBestMove(game);

            // Apply rotations
            for (int i = 0; i < move.rotation; i++) {
                game.rotate();
            }

            // Move to target x position
            tetris::Position current_pos = game.getCurrentPosition();
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

        // Update game
        if (elapsed >= update_interval && !auto_play) {
            game.update();
            last_update = now;
        }

        // Render
        if (game.getState() == tetris::GameState::PLAYING) {
            renderer.render(game);
        } else {
            renderer.renderGameOver(game);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    renderer.cleanup();
    return 0;
}

