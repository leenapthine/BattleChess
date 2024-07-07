// Filename: familiar.cpp
// Description: Implementation of the Familiar class, which defines the behavior and movement rules of the Familiar piece in our chess game.

// Main Classes:
// - Familiar: Represents the Familiar piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Familiar::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Familiar can legally move to the specified target position on the chessboard.
// - void Familiar::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Familiar on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The Familiar is a level 2 Knight piece, that belongs to the Wizard Guild
//   It moves the same as a knight in an L shape comprised of exactly 3 squares distance.
//   Familiar movement ignores occupied spaces in its path.
// - It captures upon moving into an occupied opponent space.
// - Familiar has a special ability "Turn to Stone"
//   Turning to stone ends the turn, and renders the familiar invincible until it makes a move again.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Familiar piece.

#include "piece.h"
#include "familiar.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the Familiar can move to a specified target position
bool Familiar::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
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

// Highlight valid moves for the Familiar on the provided board
void Familiar::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int currentRow = currentPosition.y / TILE_SIZE;
    int currentCol = currentPosition.x / TILE_SIZE;

    // Highlight the current position in red
    board[currentRow][currentCol].setHighlight(true, sf::Color::Red);

    // Define all possible move positions for the Familiar
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
};

// Set Familiars stone value: "Turn to Stone"
void Familiar::setStone(bool value)
{
    stone = value;
}