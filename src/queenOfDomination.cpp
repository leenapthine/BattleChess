// Filename: QueenOfDomination.cpp
// Description: Implementation of the QueenOfDomination class, which defines the behavior and movement rules of the QueenOfDomination piece in our chess game.

// Main Classes:
// - QueenOfDomination: Represents the QueenOfDomination piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool QueenOfDomination::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the QueenOfDomination can legally move to the specified target position on the chessboard.
// - void QueenOfDomination::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the QueenOfDomination on the provided chessboard, considering its current position and game rules.
// - void QueenOfDomination::highlightAdjacentFriendlyPieces(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
//   Highlights all friendly units in its perimeter in green.
// - void QueenOfDomination::toggleAdjacentFriendlyHighlights(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
//   Toggle the highlighting of friendly units off and on.
// - void QueenOfDomination::clearHighlights(std::vector<std::vector<Square>> &board) const
//   Clear highlights.
// - void QueenOfDomination::applyDominationAbility(Piece *targetPiece, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager) const
//   Grants a friendly unit Queen abilities until end of turn.
// - bool QueenOfDomination::hasUsedAbility() const
//   Track that applyDominationAbility is only used once per turn.
// - void QueenOfDomination::returnOriginalSprite(std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager)
//   Return regular sprite ability to dominated sprite at the end of turn.

// Special Featudres or Notes:
// - The QueenOfDomination is a level 2 Queen piece belonging to the BeastMaster guild.
// - The QueenOfDomination moves in straight, or vertical line in any direction, so long as the path is unobstructed.
// - The QueenOfDomination captures on movement into a space occupied by an opponent piece.
// - Has a special ability applydominationAbility that grants Queen powers to friendly units in its perimeter.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the QueenOfDomination piece.

#include "queenOfDomination.h"
#include "utility.h"
#include "pieceFactory.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <cmath>
#include <iostream>

// Determine if the QueenOfDomination can move to a specified target position
bool QueenOfDomination::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // QueenOfDomination can only move in straight lines and diagnal lines
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

// Highlight valid moves for the QueenOfDomination on the provided board
void QueenOfDomination::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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

void QueenOfDomination::highlightAdjacentFriendlyPieces(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    for (const auto &piece : pieces)
    {
        if (piece->getColor() == getColor() && piece->getPosition() != currentPosition)
        {
            float distance = std::sqrt(std::pow(piece->getPosition().x - currentPosition.x, 2) + std::pow(piece->getPosition().y - currentPosition.y, 2));

            if (distance <= TILE_SIZE * 1.5) // Adjust this threshold if needed
            {
                int row = static_cast<int>(piece->getPosition().y / TILE_SIZE);
                int col = static_cast<int>(piece->getPosition().x / TILE_SIZE);

                if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
                {
                    board[row][col].setHighlight(true, sf::Color::Green);
                    std::cout << "Highlighted square at: (" << piece->getPosition().x << ", " << piece->getPosition().y << ")" << std::endl;
                }
                else
                {
                    std::cerr << "Calculated board position out of bounds: (" << row << ", " << col << ")" << std::endl;
                }
            }
        }
    }
}

void QueenOfDomination::toggleAdjacentFriendlyHighlights(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    if (highlightsActive)
    {
        clearHighlights(board);
    }
    else
    {
        clearHighlights(board);
        highlightAdjacentFriendlyPieces(board, pieces);
        highlightsActive = true;
    }
}

void QueenOfDomination::clearHighlights(std::vector<std::vector<Square>> &board) const
{
    for (auto &row : board)
    {
        for (auto &square : row)
        {
            if (square.getHighlight())
            {
                square.setHighlight(false);
            }
        }
    }
    highlightsActive = false;
}

void QueenOfDomination::applyDominationAbility(Piece *targetPiece, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager) const
{
    // std::cout << "applyDominationAbility." << std::endl;

    if (!targetPiece || abilityUsed)
    {
        // Return if the target piece is invalid or the ability has been used
        std::cerr << "Ability already used or invalid target piece." << std::endl;
        return;
    }

    // Store the original type and position of the target piece
    originalID = targetPiece->getId();
    originalType = targetPiece->getType();
    sf::Vector2f originalPosition = targetPiece->getPosition();

    // Retrieve the original texture from the TextureManager
    std::string textureName = (originalColor == Piece::Color::White) ? "White" + originalType : "Black" + originalType;
    sf::Texture *originalTexture = textureManager.getTexture(textureName);

    if (!originalTexture)
    {
        std::cerr << "Failed to get the texture for: " << textureName << std::endl;
        return;
    }

    // Create a new QueenOfDomination with the original texture
    auto tempQueen = std::make_unique<QueenOfDomination>(originalID, *originalTexture, originalPosition, originalColor);

    // Disable the ability for the newly created queen
    tempQueen->abilityUsed = true;
    // Set originalTypes for tempQueen
    tempQueen->originalType = this->originalType;
    tempQueen->originalTexture = this->originalTexture;

    // Return originalType to None
    originalType = "None";

    // Find and replace the target piece
    auto it = std::find_if(pieces.begin(), pieces.end(),
                           [&targetPiece](const std::unique_ptr<Piece> &piece)
                           {
                               return piece.get() == targetPiece; // Compare the pointer itself
                           });

    if (it != pieces.end())
    {
        // Remove the original piece
        pieces.erase(it);

        // Add the new QueenOfDomination at the same position
        pieces.push_back(std::move(tempQueen));

        std::cout << "Selected piece becomes a Queen until next turn!!" << std::endl;
    }
    else
    {
        std::cerr << "Failed to find the target piece in the vector." << std::endl;
    }

    float scaleFactor = TILE_SIZE / static_cast<float>(std::max(textureManager.getTexture("WhiteRook")->getSize().x, textureManager.getTexture("WhiteRook")->getSize().y));

    // Scale each piece's sprite to fit the tile size
    for (auto &piece : pieces)
    {
        piece->getSprite().setScale(scaleFactor, scaleFactor);
    }

    // Mark this queen's ability as used
    abilityUsed = true;

    // At the end of the turn, replace the QueenOfDomination with the original piece type
    // This could be handled by the game loop or another function that restores the original piece
}

bool QueenOfDomination::hasUsedAbility() const
{
    return abilityUsed;
}

// Implement the return to original sprite functionality
void QueenOfDomination::returnOriginalSprite(std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager)
{
    if (originalType == "None")
    {
        return;
    }

    // Use the createPiece function to create the original piece
    auto originalPiece = createPiece(originalID, originalType, textureManager, getPosition(), originalColor);
    if (!originalPiece)
    {
        std::cerr << "Failed to create original piece." << std::endl;
        return;
    }

    // Find the QueenOfDomination in the pieces vector
    auto it = std::find_if(pieces.begin(), pieces.end(), [this](const std::unique_ptr<Piece> &piece)
                           { return piece.get() == this; });

    if (it != pieces.end())
    {
        // Replace the QueenOfDomination with the original piece
        *it = std::move(originalPiece);
    }
}