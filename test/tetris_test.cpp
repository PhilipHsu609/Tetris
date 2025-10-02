#include <gtest/gtest.h>
#include <tetris/board.hpp>
#include <tetris/game.hpp>
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

