// Filename: pawn.cpp
// Description: Implementation of the Pawn class, which defines the behavior and movement rules of the Pawn piece in our chess game.

// Main Classes:
// - Pawn: Represents the Pawn piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Pawn::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Pawn can legally move to the specified target position on the chessboard.
// - void Pawn::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Pawn on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The Pawn moves forward one square but has the option to move two squares on its first move.
// - It captures diagonally one square ahead, under specific conditions.
// - Currently does not contain logic for a Pawn promotion.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Pawn piece.

#include "pawn.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the Pawn can move to a specified target position
bool Pawn::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

    // Ensure target is within the board bounds
    if (target.x < 0 || target.x >= BOARD_SIZE * TILE_SIZE || target.y < 0 || target.y >= BOARD_SIZE * TILE_SIZE)
    {
        return false;
    }

    // Check for forward movement
    if (target.x == currentPosition.x)
    {
        // Single step forward
        if (target.y == currentPosition.y + direction * TILE_SIZE)
        {
            return !getPieceAtPosition(target, pieces);
        }
        // Double step forward on first move
        else if (target.y == currentPosition.y + 2 * direction * TILE_SIZE &&
                 ((getColor() == Piece::Color::White && currentPosition.y == 1 * TILE_SIZE) ||
                  (getColor() == Piece::Color::Black && currentPosition.y == 6 * TILE_SIZE)))
        {
            sf::Vector2f intermediatePosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
            return !getPieceAtPosition(target, pieces) && !getPieceAtPosition(intermediatePosition, pieces);
        }
    }

    // Check for diagonal capture
    if (std::abs(target.x - currentPosition.x) == TILE_SIZE && target.y == currentPosition.y + direction * TILE_SIZE)
    {
        return isOpponentPiece(target, pieces, getColor());
    }

    return false;
}

// Highlight valid moves for the Pawn on the provided board
void Pawn::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

    // Highlight single step forward
    sf::Vector2f singleStep = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
    if (singleStep.y >= 0 && singleStep.y < BOARD_SIZE * TILE_SIZE && canMoveTo(singleStep, pieces))
    {
        board[static_cast<int>(singleStep.y / TILE_SIZE)][static_cast<int>(singleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight double step forward on first move
    sf::Vector2f doubleStep = sf::Vector2f(currentPosition.x, currentPosition.y + 2 * direction * TILE_SIZE);
    if (doubleStep.y >= 0 && doubleStep.y < BOARD_SIZE * TILE_SIZE &&
        ((getColor() == Piece::Color::White && currentPosition.y == 1 * TILE_SIZE) ||
         (getColor() == Piece::Color::Black && currentPosition.y == 6 * TILE_SIZE)) &&
        canMoveTo(doubleStep, pieces))
    {
        board[static_cast<int>(doubleStep.y / TILE_SIZE)][static_cast<int>(doubleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight diagonal captures
    sf::Vector2f diagonalLeft = sf::Vector2f(currentPosition.x - TILE_SIZE, currentPosition.y + direction * TILE_SIZE);
    if (diagonalLeft.x >= 0 && diagonalLeft.x < BOARD_SIZE * TILE_SIZE && diagonalLeft.y >= 0 && diagonalLeft.y < BOARD_SIZE * TILE_SIZE &&
        canMoveTo(diagonalLeft, pieces))
    {
        board[static_cast<int>(diagonalLeft.y / TILE_SIZE)][static_cast<int>(diagonalLeft.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
    }

    sf::Vector2f diagonalRight = sf::Vector2f(currentPosition.x + TILE_SIZE, currentPosition.y + direction * TILE_SIZE);
    if (diagonalRight.x >= 0 && diagonalRight.x < BOARD_SIZE * TILE_SIZE && diagonalRight.y >= 0 && diagonalRight.y < BOARD_SIZE * TILE_SIZE &&
        canMoveTo(diagonalRight, pieces))
    {
        board[static_cast<int>(diagonalRight.y / TILE_SIZE)][static_cast<int>(diagonalRight.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
    }
}