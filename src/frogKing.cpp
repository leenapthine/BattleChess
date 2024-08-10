// Filename: FrogKing.cpp
// Description: Implementation of the FrogKing class, which defines the behavior and movement rules of the FrogKing piece in our chess game.

// Main Classes:
// - FrogKing: Represents the FrogKing piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool FrogKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the FrogKing can legally move to the specified target position on the chessboard.
// - void FrogKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the FrogKing on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - FrogKing is a level 2 King piece belonging to the Beast Master guild.
// - The FrogKing moves one square in any direction, but can 'hop' an extra space vertically and horizontally.
// - It captures on movement into a space occupied by an opponent piece.
// - Currently does not contain logic for the King / Rook 'castling' function.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the FrogKing piece.

#include "piece.h"
#include "frogKing.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the FrogKing can move to a specified target position
bool FrogKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 0 && dy == 1) || (dx == 0 && dy == 2) || (dx == 0 && dy == -1) || (dx == 0 && dy == -2) || (dx == 1 && dy == 0) || (dx == 2 && dy == 0) || (dx == 1 && dy == -1) || (dx == 2 && dy == -1) || (dx == 1 && dy == 1) || (dx == 2 && dy == 1) || (dx == -1 && dy == 0) || (dx == -2 && dy == 0) || (dx == -1 && dy == 1) || (dx == -1 && dy == 2) || (dx == -1 && dy == -1) || (dx == -2 && dy == -1))
    {
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false;
}

// Highlight valid moves for the FrogKing on the provided board
void FrogKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    std::vector<sf::Vector2f> moves = {
        {currentPosition.x, currentPosition.y + TILE_SIZE},
        {currentPosition.x, currentPosition.y - TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y},
        {currentPosition.x - TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y},
        {currentPosition.x + TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x, currentPosition.y + TILE_SIZE * 2},
        {currentPosition.x, currentPosition.y - TILE_SIZE * 2},
        {currentPosition.x - TILE_SIZE * 2, currentPosition.y},
        {currentPosition.x - TILE_SIZE * 2, currentPosition.y - TILE_SIZE * 2},
        {currentPosition.x - TILE_SIZE * 2, currentPosition.y + TILE_SIZE * 2},
        {currentPosition.x + TILE_SIZE * 2, currentPosition.y},
        {currentPosition.x + TILE_SIZE * 2, currentPosition.y + TILE_SIZE * 2},
        {currentPosition.x + TILE_SIZE * 2, currentPosition.y - TILE_SIZE * 2}};

    for (const auto &move : moves)
    {
        int col = move.x / TILE_SIZE;
        int row = move.y / TILE_SIZE;

        // space is occupied by an opponent piece
        if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && canMoveTo(move, pieces) && isOpponentPiece(move, pieces, getColor()))
        {
            board[row][col].setHighlight(true, sf::Color::Red);
        }
        // space is unoccupied
        else if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && canMoveTo(move, pieces))
        {
            board[row][col].setHighlight(true);
        }
    }
};