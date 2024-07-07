// Filename: pawnHopper.cpp
// Description: Implementation of the PawnHopper class, which defines the behavior and movement rules of the PawnHopper piece in our chess game.

// Main Classes:
// - PawnHopper: Represents the PawnHopper piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool PawnHopper::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the PawnHopper can legally move to the specified target position on the chessboard.
// - void PawnHopper::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the PawnHopper on the provided chessboard, considering its current position and game rules.
// - void PawnHopper::captureHoppedPiece(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) const:
//   logic for capturing when hopping over an enemy unit

// Special Features or Notes:
// - The PawnHopper is a level 2 Pawn piece that belongs to the BeastMaster Guild
//   It moves forward one or two squares.
// - It captures diagonally one square ahead, under specific conditions.
// - It also captures by hopping over enemy pieces.
// - Currently does not contain logic for a PawnHopper promotion.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the PawnHopper piece.

#include "pawnHopper.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the PawnHopper can move to a specified target position
bool PawnHopper::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
        // Single step or double step forward
        if (target.y == currentPosition.y + direction * TILE_SIZE || target.y == currentPosition.y + 2 * direction * TILE_SIZE)
        {
            sf::Vector2f intermediatePosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
            return !getPieceAtPosition(target, pieces);
        }
    }

    // Check for diagonal capture
    if (std::abs(target.x - currentPosition.x) == TILE_SIZE && target.y == currentPosition.y + direction * TILE_SIZE)
    {
        return isOpponentPiece(target, pieces, getColor());
    }

    return false;
}

// Highlight valid moves for the PawnHopper on the provided board
void PawnHopper::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

    // Highlight single step forward
    sf::Vector2f singleStep = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
    if (singleStep.y >= 0 && singleStep.y < BOARD_SIZE * TILE_SIZE && canMoveTo(singleStep, pieces))
    {
        board[static_cast<int>(singleStep.y / TILE_SIZE)][static_cast<int>(singleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight double step forward
    sf::Vector2f doubleStep = sf::Vector2f(currentPosition.x, currentPosition.y + 2 * direction * TILE_SIZE);
    if (doubleStep.y >= 0 && doubleStep.y < BOARD_SIZE * TILE_SIZE &&
        canMoveTo(doubleStep, pieces))
    {
        sf::Vector2f intermediatePosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
        if (!getPieceAtPosition(intermediatePosition, pieces))
        {
            board[static_cast<int>(doubleStep.y / TILE_SIZE)][static_cast<int>(doubleStep.x / TILE_SIZE)].setHighlight(true);
        }
        else
        {
            board[static_cast<int>(doubleStep.y / TILE_SIZE)][static_cast<int>(doubleStep.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
        }
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

// If moving two spaces forward, passing over an enemy unit, capture the enemy unit
void PawnHopper::captureHoppedPiece(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f;

    if (target.x == currentPosition.x && std::abs(target.y - currentPosition.y) == 2 * TILE_SIZE)
    {
        sf::Vector2f hoppedPosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
        for (auto &piece : pieces)
        {
            if (piece->getPosition() == hoppedPosition && piece->getColor() != getColor())
            {
                // Find the piece at the hopped position and remove it
                auto it = std::remove_if(pieces.begin(), pieces.end(),
                                         [&](const std::unique_ptr<Piece> &p)
                                         {
                                             return p->getPosition() == hoppedPosition;
                                         });
                pieces.erase(it, pieces.end());
                break; // Exit the loop after capturing the piece
            }
        }
    }
}