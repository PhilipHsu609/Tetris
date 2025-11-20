#include <tetris/ai.hpp>
#include <tetris/game.hpp>
#include <tetris/multiplayer.hpp>
#include <tetris/renderer.hpp>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

void printUsage(const char *program_name) {
    std::cout << "Usage: " << program_name << " [num_players]\n";
    std::cout << "  num_players: Number of AI players (1-8, default: 2)\n";
    std::cout << "\nControls:\n";
    std::cout << "  R - Reset game(s)\n";
    std::cout << "  Q - Quit\n";
    std::cout << "\nFor single player mode (num_players=1):\n";
    std::cout << "  A - Toggle AI auto-play\n";
    std::cout << "  Arrow keys - Move piece\n";
    std::cout << "  Up - Rotate piece\n";
    std::cout << "  Space - Hard drop\n";
}

int main(int argc, char *argv[]) {
    int num_players = 2; // Default to 2 players

    // Parse command line arguments
    if (argc > 1) {
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        num_players = std::atoi(argv[1]);
        if (num_players < 1 || num_players > 8) {
            std::cerr << "Error: Number of players must be between 1 and 8\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    tetris::Renderer renderer;
    renderer.init();

    bool running = true;

    if (num_players == 1) {
        // Single player mode with manual control option
        tetris::Game game;
        tetris::AI ai;
        bool auto_play = true; // Start in auto-play mode by default
        auto last_update = std::chrono::steady_clock::now();
        int update_interval = 500; // ms

        while (running) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                               now - last_update)
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

            std::this_thread::sleep_for(
                std::chrono::milliseconds(16)); // ~60 FPS
        }
    } else {
        // Multi-player mode - all AI
        tetris::MultiPlayerGame mp_game(num_players);

        while (running) {
            // Handle input
            int ch = getch();
            switch (ch) {
            case 'q':
            case 'Q':
                running = false;
                break;
            case 'r':
            case 'R':
                mp_game.reset();
                break;
            }

            // Update all games
            mp_game.update();

            // Render all games
            renderer.renderMultiPlayer(mp_game);

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100)); // Slower for multi-player
        }
    }

    renderer.cleanup();
    return 0;
}

