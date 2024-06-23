// Filename: rook.cpp
// Description: Implementation of the Rook class, which defines the behavior and movement rules of the Rook piece in our chess game.

// Main Classes:
// - Rook: Represents the Rook piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Rook::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Rook can legally move to the specified target position on the chessboard.
// - void Rook::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Rook on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The Rook moves in straight lines in any direction, so long as the path is unobstructed.
// - The Rook captures on movement into a space occupied by an opponent piece.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Rook piece.

#include "rook.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the Rook can move to a specified target position
bool Rook::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Rook can only move in straight lines
    if (target.x != getPosition().x && target.y != getPosition().y)
    {
        return false;
    }

    // Check if the path is clear
    if (!isPathClear(currentPosition, target, pieces))
    {
        return false;
    }

    // Check if the target position is occupied by an opponent's piece
    return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
}

// Highlight valid moves for the Rook on the provided board
void Rook::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Vector2f horizontalTarget(i * TILE_SIZE, currentPosition.y);

        // Iterates through all valid movement spaces, and highlights according to a movement (yellow) or a capture (red)
        if (canMoveTo(horizontalTarget, pieces))
        {
            if (!isOpponentPiece(horizontalTarget, pieces, getColor()))
            {
                board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true);
            }
            else if (isOpponentPiece(horizontalTarget, pieces, getColor()))
            {
                board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true, sf::Color::Red);
            }
        }

        sf::Vector2f verticalTarget(currentPosition.x, i * TILE_SIZE);

        // Iterates through all valid movement spaces, and highlights according to a movement (yellow) or a capture (red)
        if (canMoveTo(verticalTarget, pieces))
        {
            if (!isOpponentPiece(verticalTarget, pieces, getColor()))
            {
                board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true);
            }
            else if (isOpponentPiece(verticalTarget, pieces, getColor()))
            {
                board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
            }
        }
    }
}