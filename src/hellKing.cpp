// Filename: HellKing.cpp
// Description: Implementation of the HellKing class, which defines the behavior and movement rules of the HellKing piece in our chess game.

// Main Classes:
// - HellKing: Represents the HellKing piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool HellKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the HellKing can legally move to the specified target position on the chessboard.
// - void HellKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the HellKing on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The HellKing moves one square in any direction.
// - It captures on movement into a space occupied by an opponent piece.
// - Currently does not contain logic for the King / Rook 'castling' function.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the HellKing piece.

#include "piece.h"
#include "hellKing.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the HellKing can move to a specified target position
bool HellKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 0 && dy == 1) || (dx == 0 && dy == -1) || (dx == 1 && dy == 0) || (dx == 1 && dy == -1) || (dx == 1 && dy == 1) || (dx == -1 && dy == 0) || (dx == -1 && dy == 1) || (dx == -1 && dy == -1))
    {
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false;
}

// Highlight valid moves for the HellKing on the provided board
void HellKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
        {currentPosition.x + TILE_SIZE, currentPosition.y - TILE_SIZE}};

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
}

// Highlight potential capture zones for the Beholder on the provided board
void HellKing::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int startX = currentPosition.x / TILE_SIZE;
    int startY = currentPosition.y / TILE_SIZE;

    // Define the possible capture positions (dx, dy) where dx and dy are combinations that sum to 3
    std::vector<std::pair<int, int>> captureOffsets =
        {
            {2, 0}, {-2, 0}, {0, 2}, {0, -2}};

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
void HellKing::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    // Iterate through each piece to find and capture the opponent's piece at the target position
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        // Check if the piece at the target position is an opponent's piece
        if ((*it)->getPosition() == target && isOpponentPiece(target, pieces, getColor()))
        {
            if (canMoveTo(target, pieces))
            {
                pieces.erase(it); // Remove the opponent's piece
                setPosition(target);
                break;
            }
            else
            {
                pieces.erase(it); // Remove the opponent's piece
                break;
            }
        }
    }
};