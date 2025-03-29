// Filename: QueenOfBones.cpp
// Description: Implementation of the QueenOfBones class, which defines the behavior and movement rules of the QueenOfBones piece in our chess game.

// Main Classes:
// - QueenOfBones: Represents the Queen piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool QueenOfBones::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the QueenOfBones can legally move to the specified target position on the chessboard.
// - void QueenOfBones::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the QueenOfBones on the provided chessboard, considering its current position and game rules.

// Special Features or Notes:
// - The QueenOfBones moves in straight, or vertical line in any direction, so long as the path is unobstructed.
// - The QueenOfBones captures on movement into a space occupied by an opponent piece.
// - The QueenOfBones is a level 2 Queen Piece that belongs to the Necromancer class.
// - Upon being captured the QueenOfBones can sacrifice 2 pawn pieces to respawn at the orginal spawn point.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the QueenOfBones piece.

#include "queenOfBones.h"
#include "textureManager.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream>

// Determine if the QueenOfBones can move to a specified target position
bool QueenOfBones::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // QueenOfBones can only move in straight lines and diagonal lines
    if ((target.x != currentPosition.x && target.y != currentPosition.y) &&
        (std::abs(target.x - currentPosition.x) != std::abs(target.y - currentPosition.y)))
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

// Highlight valid moves for the QueenOfBones on the provided board
void QueenOfBones::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
                    if (isPathClear(currentPosition, newPosition, pieces) &&
                        (!getPieceAtPosition(newPosition, pieces) || isOpponentPiece(newPosition, pieces, currentPlayerColor)))
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

// Revive method for the QueenOfBones
std::vector<Piece *> QueenOfBones::revive(std::vector<std::unique_ptr<Piece>> &pieces)
{
    std::vector<Piece *> sacrificablePawns;

    for (const auto &piece : pieces)
    {
        if (piece->getColor() == getColor() &&
            (piece->getType() == "Pawn" || piece->getType() == "NecroPawn"))
        {
            // Add each pawn to the vector
            sacrificablePawns.push_back(piece.get());
        }
    }

    return sacrificablePawns;
}

void QueenOfBones::handlePawnSacrifice(Piece *selectedPawn, std::vector<std::unique_ptr<Piece>> &pieces)
{
    sf::Vector2f targetPosition = selectedPawn->getPosition();
    std::cout << "Sacrificing Pawn at position: (" << targetPosition.x << ", " << targetPosition.y << ")" << std::endl;

    // Find the pawn to remove
    auto it = std::remove_if(pieces.begin(), pieces.end(),
                             [&](const std::unique_ptr<Piece> &piece)
                             {
                                 return piece.get() == selectedPawn;
                             });
    pieces.erase(it, pieces.end());

    // Decrement the number of pawns to sacrifice
    --pawnsToSacrifice;
}

void QueenOfBones::respawnAtOriginalPosition(std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager)
{
    // Determine respawn position based on the color of the QueenOfBones
    sf::Vector2f respawnPosition = (getColor() == Piece::Color::White)
                                       ? sf::Vector2f(4 * TILE_SIZE, 0)              // White Queen's position (d1)
                                       : sf::Vector2f(4 * TILE_SIZE, 7 * TILE_SIZE); // Black Queen's position (d8)

    // Check if the spawn point is occupied
    if (getPieceAtPosition(respawnPosition, pieces) == nullptr)
    {
        std::cout << "Respawning QueenOfBones at position: (" << respawnPosition.x << ", " << respawnPosition.y << ")" << std::endl;

        // Retrieve the texture from the TextureManager based on the color of the QueenOfBones
        std::string textureName = (getColor() == Piece::Color::White) ? "WhiteQueenOfBones" : "BlackQueenOfBones";
        sf::Texture *queenTexture = textureManager.getTexture(textureName);

        // Handle the case where the texture is not found
        if (queenTexture == nullptr)
        {
            throw std::runtime_error("Texture not found: " + textureName);
        }

        // Create a new QueenOfBones and add it to the pieces vector
        pieces.push_back(std::make_unique<QueenOfBones>(this->getId(), *queenTexture, respawnPosition, getColor()));

        // Scale each piece's sprite to fit the tile size
        float scaleFactor = TILE_SIZE / static_cast<float>(std::max(queenTexture->getSize().x, queenTexture->getSize().y));
        for (auto &piece : pieces)
        {
            piece->getSprite().setScale(scaleFactor, scaleFactor);
        }
    }
    else
    {
        std::cout << "Spawn point occupied. Respawn failed." << std::endl;
    }
}