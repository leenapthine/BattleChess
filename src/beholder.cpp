// Filename: beholder.cpp
// Description: Implementation of the Beholder class, which defines the behavior and movement rules of the Beholder piece in our chess game.

// Main Classes:
// - Beholder: Represents the Beholder piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Beholder::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Beholder can legally move to the specified target position on the chessboard.
// - void Beholder::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Beholder on the provided chessboard, considering its current position and game rules.
// - void Beholder::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid capture options for the Beholder on the provided chessboard, considering its current position and game rules.
// - void Beholder::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces):
//   Captures an enemy piece. Overwrites the capture function from the Piece class.

// Special Features or Notes:
// - The Beholder moves up/down/right/left one square.
//   However, Beholder movement cannot capture enemy pieces by moving into their space.
// - Beholder captures by launching projectiles, and can use a capture ability rather than a movement ability during the turn.
//   Capturing can target any opponent piece exactly 2 or 3 squares distance away, regardless of occupied spaces within the 3 square radius.
// - Beholder is a level 2 Rook piece that belongs to the HellSpawn race in our Battle Chess game.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Beholder piece.

#include "beholder.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the Beholder can move to a specified target position
bool Beholder::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 0 && dy == 1) || (dx == 0 && dy == -1) || (dx == 1 && dy == 0) || (dx == -1 && dy == 0))
    {
        return !getPieceAtPosition(target, pieces);
    }
    return false;
}

// Highlight valid moves for the Beholder on the provided board
void Beholder::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
void Beholder::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int startX = currentPosition.x / TILE_SIZE;
    int startY = currentPosition.y / TILE_SIZE;

    // Define the possible capture positions (dx, dy) where dx and dy are combinations that sum to 3
    std::vector<std::pair<int, int>> captureOffsets =
        {
            {3, 0}, {2, 0}, {2, 1}, {1, 1}, {1, 2}, {0, 2}, {0, 3}, {0, 2}, {-3, 0}, {-2, 0}, {-2, -1}, {-1, -1}, {-1, -2}, {0, -2}, {0, -3}, {-2, 1}, {-1, 1}, {-1, 2}, {1, -1}, {1, -2}, {2, -1}};

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
void Beholder::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
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
}