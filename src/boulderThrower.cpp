// Filename: boulderThrower.cpp
// Description: Implementation of the BoulderThrower class, which defines the behavior and movement rules of the BoulderThrower piece in our chess game.

// Main Classes:
// - BoulderThrower: Represents the BoulderThrower piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool BoulderThrower::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the BoulderThrower can legally move to the specified target position on the chessboard.
// - void BoulderThrower::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the BoulderThrower on the provided chessboard, considering its current position and game rules.
// - void BoulderThrower::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid capture options for the BoulderThrower on the provided chessboard, considering its current position and game rules.
// - void BoulderThrower::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces):
//   Captures an enemy piece. Overwrites the capture function from the Piece class.

// Special Features or Notes:
// - The BoulderThrower moves in straight lines as long as unobstructed by an occupied space, similiar to the Rook piece.
//   However, BoulderThrower movement cannot capture enemy pieces.
// - BoulderThrower captures by launching projectiles, and can use a capture ability rather than a movement ability during the turn.
//   Capturing can target any opponent piece exactly 3 squares distance away, regardless of occupied spaces within the 3 square radius.
// - BoulderThrower is a level 2 Rook piece that belongs to the Beast Master race in our Battle Chess game.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the BoulderThrower piece.

#include "boulderThrower.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the BoulderThrower can move to a specified target position
bool BoulderThrower::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // BoulderThrower can only move in straight lines
    if (target.x != getPosition().x && target.y != getPosition().y)
    {
        return false;
    }

    // Check if the path is clear
    if (!isPathClear(currentPosition, target, pieces))
    {
        return false;
    }

    // Check if the target position is occupied by any piece (friendly or enemy)
    if (getPieceAtPosition(target, pieces))
    {
        return false;
    }

    return true;
}

// Highlight valid moves for the BoulderThrower on the provided board
void BoulderThrower::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Iterate through each column and row on the board
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        // Check and highlight horizontal move to each column in the current row
        sf::Vector2f horizontalTarget(i * TILE_SIZE, currentPosition.y);
        if (canMoveTo(horizontalTarget, pieces))
        {
            board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true);
        }

        // Check and highlight vertical move to each row in the current column
        sf::Vector2f verticalTarget(currentPosition.x, i * TILE_SIZE);
        if (canMoveTo(verticalTarget, pieces))
        {
            board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true);
        }
    }
}

// Highlight potential capture zones for the BoulderThrower on the provided board
void BoulderThrower::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int startX = currentPosition.x / TILE_SIZE;
    int startY = currentPosition.y / TILE_SIZE;

    // Define the possible capture positions (dx, dy) where dx and dy are combinations that sum to 3
    std::vector<std::pair<int, int>> captureOffsets =
        {
            {3, 0}, {2, 1}, {1, 2}, {0, 3}, {-3, 0}, {-2, -1}, {-1, -2}, {0, -3}, {-2, 1}, {-1, 2}, {1, -2}, {2, -1}};

    // Iterate through each capture offset to determine and highlight capture positions
    for (const auto &offset : captureOffsets)
    {
        int col = startX + offset.first;
        int row = startY + offset.second;
        sf::Vector2f target(col * TILE_SIZE, row * TILE_SIZE);

        // Check if the position is within the board bounds
        if ((col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE))
        {
            // Highlight squares where BoulderThrower can capture opponent's piece
            if (isOpponentPiece(target, pieces, getColor()))
            {
                board[row][col].setHighlight(true, sf::Color::Red);
            }
        }
    }
}

// Execute the capture of an opponent piece located at the specified target position
void BoulderThrower::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    // Iterate through each piece to find and capture the opponent's piece at the target position
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        // Check if the piece at the target position is an opponent's piece
        if ((*it)->getPosition() == target && isOpponentPiece(target, pieces, getColor()))
        {
            pieces.erase(it); // Remove the opponent's piece
            break;
        }
    }
}