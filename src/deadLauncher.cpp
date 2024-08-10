// Filename: deadLauncher.cpp
// Description: Implementation of the DeadLauncher class, which defines the behavior and movement rules of the DeadLauncher piece in our chess game.

// Main Classes:
// - DeadLauncher: Represents the DeadLauncher piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool DeadLauncher::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the DeadLauncher can legally move to the specified target position on the chessboard.
// - void DeadLauncher::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the DeadLauncher on the provided chessboard, considering its current position and game rules.
// - void DeadLauncher::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid capture options for the DeadLauncher on the provided chessboard, considering its current position and game rules.
// - void DeadLauncher::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces):
//   Captures an enemy piece. Overwrites the capture function from the Piece class.

// Special Features or Notes:
// - The DeadLauncher moves in straight lines as long as unobstructed by an occupied space, similiar to the Rook piece.
// - DeadLauncher captures both by launching projectiles (if a pawn is loaded into it) and by moving into a space occupied by an enemy,
//   It can use a launch ability rather than a movement ability during the turn.
//   Capturing by launching a pawn can target any opponent piece exactly 3 squares distance away, regardless of occupied spaces within the 3 square radius.
// - DeadLauncher is a level 2 Rook piece that belongs to the Necromancer race in our Battle Chess game.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the DeadLauncher piece.

#include "deadLauncher.h"
#include "pawn.h"
#include "necroPawn.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream>

// Determine if the DeadLauncher can move to a specified target position
bool DeadLauncher::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // DeadLauncher can only move in straight lines (either horizontally or vertically)
    if (target.x != currentPosition.x && target.y != currentPosition.y)
    {
        return false;
    }

    // Check if the path is clear for movement
    if (!isPathClear(currentPosition, target, pieces))
    {
        return false;
    }

    // Allow moving to unoccupied spaces
    Piece *targetPiece = getPieceAtPosition(target, pieces);
    if (!targetPiece)
    {
        return true; // The space is unoccupied and valid for movement
    }

    // Check if a pawn is loaded and target is within the throwing range
    if (pawnLoaded && isTargetWithinRange(target))
    {
        // The DeadLauncher can throw the pawn to capture an opponent piece
        return isOpponentPiece(target, pieces, getColor());
    }

    // Normal capturing of opponent pieces without using a loaded pawn
    if (isOpponentPiece(target, pieces, getColor()))
    {
        return true;
    }

    // Allow capturing/loading of friendly pawns in the perimeter
    if (targetPiece->getColor() == getColor() &&
        (targetPiece->getType() == "Pawn" || targetPiece->getType() == "NecroPawn"))
    {
        return true; // Allow loading of the friendly pawn
    }

    return false; // Default case if none of the above conditions are met
}

// Highlight valid moves for the DeadLauncher on the provided board
void DeadLauncher::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Convert the floating-point coordinates to integer indices
    int row = static_cast<int>(currentPosition.y / TILE_SIZE);
    int col = static_cast<int>(currentPosition.x / TILE_SIZE);

    // Ensure the indices are within the valid range of the board dimensions
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        // Highlight the current position in green
        board[row][col].setHighlight(true, sf::Color::Green);
    }

    // Iterate through each column and row on the board
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        // Check and highlight horizontal move to each column in the current row
        sf::Vector2f horizontalTarget(i * TILE_SIZE, currentPosition.y);
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

        // Check and highlight vertical move to each row in the current column
        sf::Vector2f verticalTarget(currentPosition.x, i * TILE_SIZE);
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

// Highlight potential capture zones for the DeadLauncher on the provided board
void DeadLauncher::highlightDeadCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces)
{
    sf::Vector2f currentPosition = getPosition();
    int startX = currentPosition.x / TILE_SIZE;
    int startY = currentPosition.y / TILE_SIZE;

    // Define the possible capture positions (dx, dy) where dx and dy are combinations that sum to 3
    std::vector<std::pair<int, int>> captureOffsets = {
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
            // Highlight squares where DeadLauncher can capture opponent's piece
            if (isOpponentPiece(target, pieces, getColor()) && pawnLoaded)
            {
                board[row][col].setHighlight(true, sf::Color::Red);
            }
        }
    }
}

// Execute the capture of an opponent piece located at the specified target position
void DeadLauncher::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    // Check if the target is within the capture range
    if (pawnLoaded && isTargetWithinRange(target))
    {
        // Capture by throwing the pawn
        auto it = std::find_if(pieces.begin(), pieces.end(),
                               [&target](const std::unique_ptr<Piece> &piece)
                               {
                                   return piece->getPosition() == target;
                               });

        if (it != pieces.end() && isOpponentPiece(target, pieces, getColor()))
        {
            pieces.erase(it);   // Remove the opponent's piece at the target position
            pawnLoaded = false; // Unload the pawn after throwing
        }
    }
    else
    {
        // Normal capture by moving into the target position
        auto it = std::find_if(pieces.begin(), pieces.end(),
                               [&target](const std::unique_ptr<Piece> &piece)
                               {
                                   return piece->getPosition() == target;
                               });

        if (it != pieces.end())
        {
            Piece *targetPiece = it->get();
            if (canMoveTo(target, pieces) && isOpponentPiece(target, pieces, getColor()))
            {
                setPosition(target);
                pieces.erase(it); // Remove the opponent's piece at the target position
            }
            else if (targetPiece && (targetPiece->getType() == "Pawn" || targetPiece->getType() == "NecroPawn"))
            {
                // Load the pawn onto the DeadLauncher
                pawnLoaded = true;
                pieces.erase(it); // Remove the friendly pawn from the board
            }
        }
    }
}

void DeadLauncher::highlightAdjacentPawns(std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board)
{
    int col = getPosition().x / TILE_SIZE;
    int row = getPosition().y / TILE_SIZE;

    for (const auto &dir : directions)
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
        {
            Piece *adjacentPiece = getPieceAtPosition(sf::Vector2f(newCol * TILE_SIZE, newRow * TILE_SIZE), pieces);
            if (adjacentPiece && (adjacentPiece->getType() == "Pawn" || adjacentPiece->getType() == "NecroPawn"))
            {
                board[newRow][newCol].setHighlight(true, sf::Color::Red);
            }
        }
    }
}

// Helper method to check if the target is within the throwing range of the DeadLauncher
bool DeadLauncher::isTargetWithinRange(const sf::Vector2f &target) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(static_cast<int>(currentPosition.x / TILE_SIZE) - static_cast<int>(target.x / TILE_SIZE));
    int dy = std::abs(static_cast<int>(currentPosition.y / TILE_SIZE) - static_cast<int>(target.y / TILE_SIZE));

    // Check if the target is within the throwing range (e.g., 3 squares away in any direction)
    return (dx + dy == 3);
}
