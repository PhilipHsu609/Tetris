#include <gtest/gtest.h>
#include <tetris/ai.hpp>
#include <tetris/board.hpp>
#include <tetris/game.hpp>
#include <tetris/multiplayer.hpp>
#include <tetris/tetromino.hpp>

// Test Tetromino creation and rotation
TEST(TetrominoTest, CreateAndRotate) {
    tetris::Tetromino piece(tetris::TetrominoType::I);
    EXPECT_EQ(piece.getType(), tetris::TetrominoType::I);
    EXPECT_EQ(piece.getRotation(), 0);

    piece.rotate();
    EXPECT_EQ(piece.getRotation(), 1);

    piece.rotate();
    EXPECT_EQ(piece.getRotation(), 2);
}

// Test Board initialization
TEST(BoardTest, Initialization) {
    tetris::Board board;
    EXPECT_EQ(board.getWidth(), 10);
    EXPECT_EQ(board.getHeight(), 20);

    // Board should be empty initially
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            EXPECT_EQ(board.getCell(x, y), 0);
        }
    }
}

// Test piece placement
TEST(BoardTest, CanPlacePiece) {
    tetris::Board board;
    tetris::Tetromino piece(tetris::TetrominoType::O);

    // Should be able to place piece at starting position
    EXPECT_TRUE(board.canPlace(piece, {4, 0}));

    // Should not be able to place piece outside boundaries
    EXPECT_FALSE(board.canPlace(piece, {-1, 0}));
    EXPECT_FALSE(board.canPlace(piece, {9, 0}));
    EXPECT_FALSE(board.canPlace(piece, {0, -1}));
}

// Test line clearing
TEST(BoardTest, ClearLines) {
    tetris::Board board;

    // Fill bottom row
    tetris::Tetromino piece(tetris::TetrominoType::I);
    for (int x = 0; x < 10; x += 4) {
        board.place(piece, {x, 19});
    }

    int cleared = board.clearLines();
    EXPECT_GT(cleared, 0);
}

// Test game initialization
TEST(GameTest, Initialization) {
    tetris::Game game;
    EXPECT_EQ(game.getScore(), 0);
    EXPECT_EQ(game.getLevel(), 1);
    EXPECT_EQ(game.getLinesCleared(), 0);
    EXPECT_EQ(game.getState(), tetris::GameState::PLAYING);
}

// Test game reset
TEST(GameTest, Reset) {
    tetris::Game game;
    game.moveLeft();
    game.moveRight();
    game.reset();

    EXPECT_EQ(game.getScore(), 0);
    EXPECT_EQ(game.getLevel(), 1);
    EXPECT_EQ(game.getLinesCleared(), 0);
    EXPECT_EQ(game.getState(), tetris::GameState::PLAYING);
}

// Test AI evaluation function
TEST(AITest, EvaluatePosition) {
    tetris::AI ai;
    tetris::Board board;
    tetris::Tetromino piece(tetris::TetrominoType::I);

    // Should be able to evaluate a valid position
    int score = ai.evaluatePosition(board, piece, {3, 0});
    EXPECT_GT(score, std::numeric_limits<int>::min());

    // Should return minimum score for invalid position
    int invalid_score = ai.evaluatePosition(board, piece, {-10, 0});
    EXPECT_EQ(invalid_score, std::numeric_limits<int>::min());
}

// Test AI can find a move
TEST(AITest, FindBestMove) {
    tetris::AI ai;
    tetris::Game game;

    tetris::AI::Move move = ai.findBestMove(game);
    
    // Should return a valid move with a score
    EXPECT_GE(move.rotation, 0);
    EXPECT_LT(move.rotation, 4);
    EXPECT_GT(move.score, std::numeric_limits<int>::min());
}

// Test multi-player game initialization
TEST(MultiPlayerTest, Initialization) {
    tetris::MultiPlayerGame mp_game(3);
    
    EXPECT_EQ(mp_game.getNumPlayers(), 3);
    EXPECT_TRUE(mp_game.isAnyPlaying());
    EXPECT_EQ(mp_game.getActivePlayers(), 3);
    
    // Check each game is initialized
    for (int i = 0; i < 3; i++) {
        const tetris::Game &game = mp_game.getGame(i);
        EXPECT_EQ(game.getState(), tetris::GameState::PLAYING);
        EXPECT_EQ(game.getScore(), 0);
    }
}

// Test multi-player game reset
TEST(MultiPlayerTest, Reset) {
    tetris::MultiPlayerGame mp_game(2);
    
    // Update a few times
    mp_game.update();
    mp_game.update();
    
    // Reset
    mp_game.reset();
    
    // All games should be back to initial state
    EXPECT_EQ(mp_game.getActivePlayers(), 2);
    for (int i = 0; i < 2; i++) {
        const tetris::Game &game = mp_game.getGame(i);
        EXPECT_EQ(game.getState(), tetris::GameState::PLAYING);
        EXPECT_EQ(game.getScore(), 0);
    }
}

// Test multi-player game update
TEST(MultiPlayerTest, Update) {
    tetris::MultiPlayerGame mp_game(2);
    
    // Games should be playing
    EXPECT_TRUE(mp_game.isAnyPlaying());
    
    // Update should not crash
    mp_game.update();
    
    // At least initially, games should still be playing
    EXPECT_TRUE(mp_game.isAnyPlaying());
}

