// Filename: beastKnight.cpp
// Description: Implementation of the beastKnight class, which defines the behavior and movement rules of the BeastKnight piece in our chess game.

// Main Classes:
// - BeastKnight: Represents the beastKnight piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool BeastKnight::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the BeastKnight can legally move to the specified target position on the chessboard.
// - void BeastKnight::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the BeastKnight on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The BeastKnight moves in an L shape, similiar to the Knight piece. However, movement distance spans 4 squares rather than 3.
//   BeastKnight movement ignores occupied squares that are not the target movment square.
// - It captures on regular movement into an occupied space.
// - BeastKnight is a level 2 Knight piece that belongs to the Beast Master race in our Battle Chess game.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the BeastKnight piece.

#include "piece.h"
#include "beastKnight.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the BeastKnight can move to a specified target position
bool BeastKnight::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 3 && dy == 1) || (dx == 1 && dy == 3))
    {
        // Ensure the target position is either empty or contains an opponent's piece
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false; // Target position is not reachable by the BeastKnight
}

// Highlight valid moves for the BeastKnight on the provided board
void BeastKnight::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Define all possible move positions for the BeastKnight
    std::vector<sf::Vector2f> moves = {
        {currentPosition.x + 3 * TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + 3 * TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x - 3 * TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x - 3 * TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y + 3 * TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y - 3 * TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y + 3 * TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y - 3 * TILE_SIZE}};

    // Iterate through each possible move and highlight valid moves on the board
    for (const auto &move : moves)
    {
        int col = move.x / TILE_SIZE;
        int row = move.y / TILE_SIZE;
        if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && canMoveTo(move, pieces) && isOpponentPiece(move, pieces, getColor()))
        {
            // Highlight squares where BeastKnight can move and capture opponent's piece
            board[row][col].setHighlight(true, sf::Color::Red);
        }
        else if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && canMoveTo(move, pieces))
        {
            // Highlight squares where BeastKnight can move
            board[row][col].setHighlight(true);
        }
    }
};