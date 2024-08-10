// Filename: QueenOfDestruction.cpp
// Description: Implementation of the QueenOfDestruction class, which defines the behavior and movement rules of the QueenOfDestruction piece in our chess game.

// Main Classes:
// - QueenOfDestruction: Represents the QueenOfDestruction piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool QueenOfDestruction::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the QueenOfDestruction can legally move to the specified target position on the chessboard.
// - void QueenOfDestruction::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the QueenOfDestruction on the provided chessboard, considering its current position and game rules.
// - void QueenOfDestruction::massDestruction(const Piece *capturingPiece, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board)
//   Upon dying, erases all enemy units in its line of sight.
// - bool QueenOfDestruction::canMoveToIgnoringPiece(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces, const Piece *ignorePiece) const
//   Track that the piece that captures the QueenOfDestruction does not block its line of sight.
// - bool QueenOfDestruction::isPathClearIgnoringPiece(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces, const Piece *ignorePiece) const
//   Works in conjunction with canMoveToIgnoringPiece.

// Special Features or Notes:
// - QueenOfDestruction is a level 2 Queen Piece that belongs to the Hellspawn guild.
// - The QueenOfDestruction moves in straight, or vertical line in any direction, so long as the path is unobstructed.
// - The QueenOfDestruction captures on movement into a space occupied by an opponent piece.
// - Upon dying, QueenOfDestruction erases all enemy units in its line of sight.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the QueenOfDestruction piece.

#include "queenOfDestruction.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the QueenOfDestruction can move to a specified target position
bool QueenOfDestruction::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // QueenOfDestruction can only move in straight lines and diagnal lines
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

// Highlight valid moves for the QueenOfDestruction on the provided board
void QueenOfDestruction::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
            else if (isOpponentPiece(horizontalTarget, pieces, currentPlayerColor))
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
            else if (isOpponentPiece(verticalTarget, pieces, currentPlayerColor))
            {
                board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
            }
        }
    }

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
                    if (isPathClear(currentPosition, newPosition, pieces) && (!getPieceAtPosition(newPosition, pieces) || isOpponentPiece(newPosition, pieces, currentPlayerColor)))
                    {
                        if (!isOpponentPiece(newPosition, pieces, currentPlayerColor))
                        {
                            board[newY][newX].setHighlight(true);
                        }
                        else if (isOpponentPiece(newPosition, pieces, currentPlayerColor))
                        {
                            board[newY][newX].setHighlight(true, sf::Color::Red);
                        }

                        // If the position is occupied by an opponent's piece, stop highlighting in this direction
                        if (getPieceAtPosition(newPosition, pieces))
                        {
                            break;
                        }
                    }
                    else
                    {
                        // If the path is not clear or the position is occupied by a friendly piece, stop highlighting in this direction
                        break;
                    }
                }
                else
                {
                    // If the new position is out of bounds, stop highlighting in this direction
                    break;
                }
            }
        }
    }
}

void QueenOfDestruction::massDestruction(const Piece *capturingPiece, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board)
{
    sf::Vector2f currentPosition = getPosition();

    // Vector to hold pointers to pieces to remove
    std::vector<Piece *> piecesToRemove;

    // Identify all pieces that can be destroyed
    for (const auto &piece : pieces)
    {
        if (piece.get() != this && piece.get() != capturingPiece) // Ensure we're not erasing the Queen or the capturing piece
        {
            sf::Vector2f currPiecePos = piece->getPosition();

            if (canMoveToIgnoringPiece(currPiecePos, pieces, capturingPiece)) // Check move validity while ignoring capturing piece
            {
                piecesToRemove.push_back(piece.get()); // Store the pointer to the piece
            }
        }
    }

    // Remove identified pieces from the original vector
    pieces.erase(
        std::remove_if(
            pieces.begin(),
            pieces.end(),
            [&piecesToRemove](const std::unique_ptr<Piece> &piece)
            {
                return std::find(piecesToRemove.begin(), piecesToRemove.end(), piece.get()) != piecesToRemove.end();
            }),
        pieces.end());

    // Highlighting for debugging visualization
    for (const auto &piece : piecesToRemove)
    {
        sf::Vector2f pos = piece->getPosition();
        int x = static_cast<int>(pos.x / TILE_SIZE);
        int y = static_cast<int>(pos.y / TILE_SIZE);

        board[y][x].setHighlight(true, sf::Color::Red);
        std::cout << "Identified piece for destruction at: (" << x << ", " << y << ")\n";
    }

    // Optionally, clear highlights after destruction
    for (auto &row : board)
    {
        for (auto &square : row)
        {
            square.setHighlight(false);
        }
    }

    std::cout << "Mass destruction completed.\n";
}

bool QueenOfDestruction::canMoveToIgnoringPiece(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces, const Piece *ignorePiece) const
{
    sf::Vector2f currentPosition = getPosition();

    // QueenOfDestruction can only move in straight lines and diagonal lines
    if ((target.x != currentPosition.x && target.y != currentPosition.y) &&
        (std::abs(target.x - currentPosition.x) != std::abs(target.y - currentPosition.y)))
    {
        return false;
    }

    // Check if the path is clear, ignoring the capturing piece
    if (!isPathClearIgnoringPiece(currentPosition, target, pieces, ignorePiece))
    {
        return false;
    }

    // Check if the target position is occupied by an opponent's piece
    return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
}

// Helper function to check if path is clear, ignoring a specific piece
bool QueenOfDestruction::isPathClearIgnoringPiece(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces, const Piece *ignorePiece) const
{
    // Calculate the direction vector by dividing the differences by TILE_SIZE
    sf::Vector2f direction = {(end.x - start.x) / TILE_SIZE, (end.y - start.y) / TILE_SIZE};

    // Normalize the direction to ensure it's either -1, 0, or 1
    direction.x = direction.x != 0 ? direction.x / std::abs(direction.x) : 0;
    direction.y = direction.y != 0 ? direction.y / std::abs(direction.y) : 0;

    // Move to the first position to check
    sf::Vector2f currentPos = start + sf::Vector2f(direction.x * TILE_SIZE, direction.y * TILE_SIZE);

    // Traverse the path until the end position is reached
    while (currentPos != end)
    {
        // Check if there's a piece at the current position, ignoring the specified piece
        if (auto piece = getPieceAtPosition(currentPos, pieces))
        {
            if (piece != ignorePiece)
            {
                return false; // Path is not clear, a piece is blocking
            }
        }

        // Move to the next position along the direction
        currentPos += sf::Vector2f(direction.x * TILE_SIZE, direction.y * TILE_SIZE);
    }

    return true; // Path is clear
}