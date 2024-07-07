// Filename: ghostKnight.cpp
// Description: Implementation of the GhostKnight class, which defines the behavior and movement rules of the GhostKnight piece in our chess game.

// Main Classes:
// - GhostKnight: Represents the GhostKnight piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool GhostKnight::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the GhostKnight can legally move to the specified target position on the chessboard.
// - void GhostKnight::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the GhostKnight on the provided chessboard, considering its current position and game rules.
// - void GhostKnight::stunAdjacentEnemies(const sf::Vector2f &position, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board)
//   logic stun enemu units in the perimeter of the target position.

// Special Features or Notes:
// - The GhostKnight is a level 2 Knight piece that belongs to the Necromancer Guild.
//   It moves the same as a knight in an L shape comprised of exactly 3 squares distance.
//   GhostKnight movement ignores occupied spaces in its path.
// - It captures upon moving into an occupied opponent space.
// - GhostKnight has a special ability "stun adjacent enemies"
//   Upon moving (or capturing) into a space, all enemies in its perimeter become stunned
//   and cannot move on the opposing players next turn.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the GhostKnight piece.

#include "piece.h"
#include "ghostKnight.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream>

// Determine if the GhostKnight can move to a specified target position
bool GhostKnight::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
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

// Highlight valid moves for the GhostKnight on the provided board
void GhostKnight::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Define all possible move positions for the GhostKnight
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
}

// Stuns all enemies in the perimeter of the new target position
void GhostKnight::stunAdjacentEnemies(const sf::Vector2f &position, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board)
{
    static const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    int col = position.x / TILE_SIZE;
    int row = position.y / TILE_SIZE;

    for (const auto &dir : directions)
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
        {
            Piece *adjacentPiece = getPieceAtPosition(sf::Vector2f(newCol * TILE_SIZE, newRow * TILE_SIZE), pieces);
            if (adjacentPiece && adjacentPiece->getColor() != getColor())
            {
                std::cout << "Adjacent piece is stunned: (" << adjacentPiece->isStunned() << ")\n";
                adjacentPiece->setStunned(true);
                std::cout << "Adjacent piece is stunned: (" << adjacentPiece->isStunned() << ")\n";
            }
        }
    }
};