// Filename: wizard.cpp
// Description: Implementation of the Wizard class, a special piece that moves diagonally and can capture opponent pieces from a disctance.

// Main Classes:
// - Wizard: Manages the movement and capturing abilities of the Wizard piece.

// Main Functions:
// - bool Wizard::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Determines if the Wizard can move to a given target position based on its movement rules.
// - void Wizard::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Wizard on the provided chessboard.
// - void Wizard::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces):
//   Captures an opponent's piece at the given target position.

// Special Features or Notes:
// - The Wizard moves diagonally similar to a Bishop in standard chess.
// - The Wizard is a Level 2 Bishop Piece belonging to the Wizard race.
// - It captures opponent pieces much like a Biahop, but it does it from disctance rather than moving into the occupied space.

// Usage or Context:
// - This file implements the behavior and unique abilities of the Wizard piece in our Battle Chess.

#include "wizard.h"
#include "globals.h"
#include "utility.h"

// Determine if the Wizard can move to a specified target position
bool Wizard::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Wizard can only move diagonally
    if (std::abs(target.x - currentPosition.x) != std::abs(target.y - currentPosition.y))
    {
        return false;
    }

    // Check if the path is clear
    return isPathClear(currentPosition, target, pieces);
}

// Highlight valid moves for the Wizard on the provided board
void Wizard::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    Piece::Color currentPlayerColor = getColor();

    // Iterate over all diagonal directions
    for (int dx = -1; dx <= 1; dx += 2)
    {
        for (int dy = -1; dy <= 1; dy += 2)
        {
            // Check valid moves in each diagonal direction
            for (float i = 1; i < BOARD_SIZE; ++i)
            {
                int newX = static_cast<int>(currentPosition.x / TILE_SIZE) + i * dx;
                int newY = static_cast<int>(currentPosition.y / TILE_SIZE) + i * dy;

                // Check if the new position is within the board bounds
                if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE)
                {
                    sf::Vector2f newPosition(newX * TILE_SIZE, newY * TILE_SIZE);

                    // Check if the path is clear and if the position is empty or occupied by an opponent's piece
                    if (isPathClear(currentPosition, newPosition, pieces))
                    {
                        if (!getPieceAtPosition(newPosition, pieces))
                        {
                            board[newY][newX].setHighlight(true);
                        }
                        else if (isOpponentPiece(newPosition, pieces, currentPlayerColor))
                        {
                            board[newY][newX].setHighlight(true, sf::Color::Red); // Highlight the capture position
                            break;                                                // Stop if an opponent's piece is found (no further moves in this direction)
                        }
                        else
                        {
                            break; // Stop if a friendly piece is found
                        }
                    }
                    else
                    {
                        break; // Stop if the path is not clear
                    }
                }
                else
                {
                    break; // Stop if out of bounds
                }
            }
        }
    }
}

// Eliminates the opponent piece in the targetted space
void Wizard::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        if ((*it)->getPosition() == target && isOpponentPiece(target, pieces, getColor()))
        {
            pieces.erase(it); // Remove the opponent's piece
            break;
        }
    }
}