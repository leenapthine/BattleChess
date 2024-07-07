// Filename: prowler.cpp

// Description: Implementation of the Prowler class, which defines the behavior and movement rules of the Prowler piece in our chess game.

// Main Classes:
// - Prowler: Represents the Prowler piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Prowler::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Prowler can legally move to the specified target position on the chessboard.
// - void Prowler::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Prowler on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The Prowler is a level 2 Knight piece that belongs to the HellSpawn Guild
//   It moves in an L shape comprised of exactly 3 squares distance.
//   Prowler movement ignores occupied spaces in its path.
// - When the Prowler captures the first time each turn, it can move again one more time.
// - It captures upon moving into an occupied opponent space.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Prowler piece.

#include "piece.h"
#include "prowler.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream>

bool Prowler::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
    {
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false;
}

void Prowler::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    std::vector<sf::Vector2f> moves = {
        {currentPosition.x + 2 * TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + 2 * TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x - 2 * TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x - 2 * TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y + 2 * TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y - 2 * TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y + 2 * TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y - 2 * TILE_SIZE}};

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
