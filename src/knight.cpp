// Filename: knight.cpp
// Description: Implementation of the Knight class, which defines the behavior and movement rules of the Knight piece in our chess game.

// Main Classes:
// - Knight: Represents the Pawn piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Knight::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Knight can legally move to the specified target position on the chessboard.
// - void Knight::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Knight on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The Knight moves in an L shape comprised of exactly 3 squares distance.
//   Knight movement ignores occupied spaces in its path.
// - It captures upon moving into an occupied opponent space.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Knight piece.

#include "piece.h"
#include "knight.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the Knight can move to a specified target position
bool Knight::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
    {
        // Ensure the target position is either empty or contains an opponent's piece
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false;
}

// Highlight valid moves for the Knight on the provided board
void Knight::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Define all possible move positions for the Knight
    std::vector<sf::Vector2f> moves = {
        {currentPosition.x + 2 * TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + 2 * TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x - 2 * TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x - 2 * TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y + 2 * TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y - 2 * TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y + 2 * TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y - 2 * TILE_SIZE}};

    // Iterate through each possible move and highlight valid moves on the board
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