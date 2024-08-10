// Filename: QueenOfIllusions.cpp
// Description: Implementation of the QueenOfIllusions class, which defines the behavior and movement rules of the QueenOfIllusions piece in our chess game.

// Main Classes:
// - QueenOfIllusions: Represents the QueenOfIllusions piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool QueenOfIllusions::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the QueenOfIllusions can legally move to the specified target position on the chessboard.
// - void QueenOfIllusions::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Queen on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The QueenOfIllusions moves in straight, or vertical line in any direction, so long as the path is unobstructed.
// - The QueenOfIllusions captures on movement into a space occupied by an opponent piece.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the QueenOfIllusions piece.

#include "queenOfIllusions.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the QueenOfIllusions can move to a specified target position
bool QueenOfIllusions::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // QueenOfIllusions can only move in straight lines and diagnal lines
    if ((target.x != getPosition().x && target.y != getPosition().y) && (std::abs(target.x - currentPosition.x) != std::abs(target.y - currentPosition.y)))
    {
        return false;
    }

    // Check if the path is clear
    if (!isPathClear(currentPosition, target, pieces))
    {
        return false;
    }

    // Check if the target position is occupied by an opponent's piece
    return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
}

// Highlight valid moves for the QueenOfIllusions on the provided board
void QueenOfIllusions::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    Piece::Color currentPlayerColor = getColor();

    // Iterates through all valid movement spaces, and highlights according to a movement (yellow) or a capture (red)
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Vector2f horizontalTarget(i * TILE_SIZE, currentPosition.y);
        if (canMoveTo(horizontalTarget, pieces))
        {
            if (!isOpponentPiece(horizontalTarget, pieces, currentPlayerColor))
            {
                board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true);
            }
            else
            {
                board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true, sf::Color::Red);
            }
        }

        sf::Vector2f verticalTarget(currentPosition.x, i * TILE_SIZE);
        if (canMoveTo(verticalTarget, pieces))
        {
            if (!isOpponentPiece(verticalTarget, pieces, currentPlayerColor))
            {
                board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true);
            }
            else
            {
                board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
            }
        }
    }

    // Highlight all friendly pawn/YoungWiz squares in Green
    for (const auto &piece : pieces)
    {
        if (piece->getColor() == currentPlayerColor && (piece->getType() == "Pawn" || piece->getType() == "YoungWiz"))
        {
            sf::Vector2f piecePosition = piece->getPosition();
            board[static_cast<int>(piecePosition.y / TILE_SIZE)][static_cast<int>(piecePosition.x / TILE_SIZE)].setHighlight(true, sf::Color::Green);
        }
    }

    // Iterate over all diagonal directions
    for (int dx = -1; dx <= 1; dx += 2)
    {
        for (int dy = -1; dy <= 1; dy += 2)
        {
            // Check valid moves in each diagonal direction
            for (int i = 1; i < BOARD_SIZE; ++i)
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
                            board[newY][newX].setHighlight(true, sf::Color::Red);
                            break; // Stop highlighting in this direction
                        }
                        else
                        {
                            break; // Friendly piece is blocking the path
                        }
                    }
                    else
                    {
                        break; // Path is not clear
                    }
                }
                else
                {
                    break; // New position is out of bounds
                }
            }
        }
    }
}

// Define the swap function
void QueenOfIllusions::swap(Piece *targetPiece, std::vector<std::vector<Square>> &board)
{
    // Get current positions
    sf::Vector2f queenPosition = getPosition();
    sf::Vector2f targetPosition = targetPiece->getPosition();

    // Update positions
    setPosition(targetPosition);
    targetPiece->setPosition(queenPosition);

    /*
        // Update the board
        int queenRow = queenPosition.y / TILE_SIZE;
        int queenCol = queenPosition.x / TILE_SIZE;
        int targetRow = targetPosition.y / TILE_SIZE;
        int targetCol = targetPosition.x / TILE_SIZE;

        board[queenRow][queenCol].setPiece(targetPiece);
        board[targetRow][targetCol].setPiece(this);
        */

    // Clear highlights
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            board[r][c].setHighlight(false);
        }
    }
}