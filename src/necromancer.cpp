// Filename: necromancer.cpp
// Description: Implementation of the Necromancer class, a unique chess piece with diagonal movement and the ability to raise fallen pawns.

// Main Classes:
// - Necromancer: Manages the movement and special ability (raising dead pawns) of the Necromancer piece.

// Main Functions:
// - bool Necromancer::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Determines if the Necromancer can move to a given target position based on its movement rules.
// - void Necromancer::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Necromancer on the provided chessboard.
// - bool Necromancer::raiseDead(const sf::Vector2f &targetPosition, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces):
//   Attempts to raise a pawn at adjacent positions to the target position.
// - sf::Texture &Necromancer::getPawnTexture(): Retrieves the appropriate pawn texture based on the Necromancer's color.
// - std::vector<sf::Vector2f> Necromancer::getAdjacentPositions(const sf::Vector2f &position) const:
//   Returns all adjacent positions to a specific given current position.

// Special Features or Notes:
// - The Necromancer can move diagonally similar to a Bishop.
// - The Necromancer is a Level 2 Bishop Piece belonging to the Necromancer Race.
// - It has a unique ability to raise fallen pawns at adjacent positions to a specified target position.

// Usage or Context:
// - This file implements the behavior and unique abilities of the Necromancer piece in our Battle Chess game.

#include "necromancer.h"
#include "globals.h"
#include "utility.h"
#include "pawn.h"
#include "textureManager.h"
#include "iostream"

// Determine if the Pawn can move to a specified target position
bool Necromancer::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Necromancer can only move diagonally
    if (std::abs(target.x - currentPosition.x) != std::abs(target.y - currentPosition.y))
    {
        return false;
    }

    // Check if the path is clear
    return isPathClear(currentPosition, target, pieces);
}

// Highlight valid moves for the Pawn on the provided board
void Necromancer::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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

// Place a pawn piece in a selected adjacent square
bool Necromancer::raiseDead(const sf::Vector2f &targetPosition, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager)
{
    bool awaitingPawnPlacement = false;
    const sf::Texture &pawnTexture = getPawnTexture(textureManager);

    // Get all adjacent positions to the target position
    std::vector<sf::Vector2f> possiblePositions = getAdjacentPositions(targetPosition);

    for (const auto &pos : possiblePositions)
    {
        // Check if the adjacent position is within board bounds
        if (pos.x >= 0 && pos.x < BOARD_SIZE * TILE_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE * TILE_SIZE)
        {
            // Check if the adjacent position is empty
            if (!getPieceAtPosition(pos, pieces))
            {
                awaitingPawnPlacement = true;
                int col = pos.x / TILE_SIZE;
                int row = pos.y / TILE_SIZE;
                // Highlight the square for pawn placement
                board[row][col].setHighlight(true, sf::Color::Green);
                std::cout << "hightlighting for pawn placement: (" << col << ", " << row << ")" << std::endl; // Debug
            }
        }
    }
    return awaitingPawnPlacement;
}

// Return the appropriate pawn texture based on the Necromancer's color
sf::Texture &Necromancer::getPawnTexture(TextureManager &textureManager) const
{
    // Determine the texture name based on the Necromancer's color
    std::string textureName = (getColor() == Piece::Color::White) ? "WhitePawn" : "BlackPawn";

    // Retrieve the texture from the TextureManager
    sf::Texture *texture = textureManager.getTexture(textureName);

    // Handle the case where the texture is not found
    if (texture == nullptr)
    {
        throw std::runtime_error("Texture not found: " + textureName);
    }

    return *texture;
}

// Return all four adjacent positions (left, right, up, down)
std::vector<sf::Vector2f> Necromancer::getAdjacentPositions(const sf::Vector2f &position) const
{
    return {
        {position.x + TILE_SIZE, position.y},
        {position.x - TILE_SIZE, position.y},
        {position.x, position.y + TILE_SIZE},
        {position.x, position.y - TILE_SIZE}};
}