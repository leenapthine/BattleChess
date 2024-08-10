// Filename: beastDruid.cpp
// Description: Implementation of the BeastDruid class, which defines the behavior and movement rules of the BeastDruid piece in our chess game.

// Main Classes:
// - BeastDruid: Represents the BeastDruid piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool BeastDruid::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the BeastDruid can legally move to the specified target position on the chessboard.
// - void BeastDruid::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the BeastDruid on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - BeastDruid is a level 2 Bishop, belonging to the BeastMaster Guild.
// - The BeastDruid moves both diagnally and anywhere its current perimeter, but is limited to only an unoccupied path.
// - It captures by entering an occupied space anywhere that it is allowed to move.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the BeastDruid piece.

#include "beastDruid.h"
#include "globals.h"
#include "utility.h"

// Determine if the BeastDruid can move to a specified target position
bool BeastDruid::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    // BeastDruid can move diagonally or one space in any direction
    if (std::abs(target.x - currentPosition.x) == std::abs(target.y - currentPosition.y))
    {
        // Check if the path is clear
        return isPathClear(currentPosition, target, pieces);
    }
    else if ((dx == 0 && dy == 1) || (dx == 0 && dy == -1) || (dx == 1 && dy == 0) || (dx == 1 && dy == -1) || (dx == 1 && dy == 1) || (dx == -1 && dy == 0) || (dx == -1 && dy == 1) || (dx == -1 && dy == -1))
    {
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    else
    {
        return false;
    }
}

// Highlight valid moves for the BeastDruid on the provided board
void BeastDruid::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
                    if (isPathClear(currentPosition, newPosition, pieces) && (!getPieceAtPosition(newPosition, pieces) || isOpponentPiece(newPosition, pieces, currentPlayerColor)))
                    {
                        if (!isOpponentPiece(newPosition, pieces, currentPlayerColor))
                        {
                            // Highlight unoccupied squares yellow where BeastDruid can move
                            board[newY][newX].setHighlight(true);
                        }
                        else if (isOpponentPiece(newPosition, pieces, currentPlayerColor))
                        {
                            // Highlight squares red where Bishop can move and capture opponent's piece
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
                { // Move anywhere in the perimeter of the BeastDruid
                    std::vector<sf::Vector2f> moves = {
                        {currentPosition.x, currentPosition.y + TILE_SIZE},
                        {currentPosition.x, currentPosition.y - TILE_SIZE},
                        {currentPosition.x - TILE_SIZE, currentPosition.y},
                        {currentPosition.x + TILE_SIZE, currentPosition.y}};

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
                    // If the new position is out of bounds, stop highlighting in this direction
                    break;
                }
            }
        }
    }
}