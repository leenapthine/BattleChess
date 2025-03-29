// Filename: hellPawn.cpp
// Description: Implementation of the HellPawn class, which defines the behavior,
// movement, and capture rules of the HellPawn piece in our chess game.

// Main Classes:
// - HellPawn: Represents the HellPawn piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool HellPawn::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the HellPawn can legally move to the specified target position on the chessboard.
// - void HellPawn::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the HellPawn on the provided chessboard, considering its current position and game rules.
// - void HellPawn::infect(const Piece *capturedPiece, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board, TextureManager &textureManager)
//   Infects the minds of enemy pieces and merges with them to take control.

// Special Features or Notes:
// - The HellPawn is a Level 2 Pawn piece belonging to the Hellspawn race.
// - The HellPawn has the movement and capture abilities of a pawn.
//   It moves forward one square but has the option to move two squares on its first move.
// - It captures diagonally one square ahead, under specific conditions.
// - HellPawn has the special infect ability, that when it captures a non-pawn piece,
//   it becomes a copy of that piece in its own allied color.
// - Currently does not contain logic for a Pawn promotion.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the HellPawn piece.

#include "king.h"
#include "hellKing.h"
#include "queen.h"
#include "queenOfIllusions.h"
#include "queenOfBones.h"
#include "queenOfDomination.h"
#include "queenOfDestruction.h"
#include "rook.h"
#include "pawn.h"
#include "pawnHopper.h"
#include "bishop.h"
#include "knight.h"
#include "wizard.h"
#include "necromancer.h"
#include "beastKnight.h"
#include "ghostKnight.h"
#include "beastDruid.h"
#include "necroPawn.h"
#include "boulderThrower.h"
#include "beholder.h"
#include "deadLauncher.h"
#include "howler.h"
#include "portal.h"
#include "familiar.h"
#include "prowler.h"
#include "hellPawn.h"
#include "youngWiz.h"
#include "ghoulKing.h"
#include "wizardKing.h"
#include "frogKing.h"
#include "utility.h"
#include "pieceFactory.h"
#include "textureManager.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream>

// Determine if the HellPawn can move to a specified target position
bool HellPawn::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

    // Ensure target is within the board bounds
    if (target.x < 0 || target.x >= BOARD_SIZE * TILE_SIZE || target.y < 0 || target.y >= BOARD_SIZE * TILE_SIZE)
    {
        return false;
    }

    // Check for forward movement
    if (target.x == currentPosition.x)
    {
        // Single step forward
        if (target.y == currentPosition.y + direction * TILE_SIZE)
        {
            return !getPieceAtPosition(target, pieces);
        }
        // Double step forward on first move
        else if (target.y == currentPosition.y + 2 * direction * TILE_SIZE &&
                 ((getColor() == Piece::Color::White && currentPosition.y == 1 * TILE_SIZE) ||
                  (getColor() == Piece::Color::Black && currentPosition.y == 6 * TILE_SIZE)))
        {
            sf::Vector2f intermediatePosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
            return !getPieceAtPosition(target, pieces) && !getPieceAtPosition(intermediatePosition, pieces);
        }
    }

    // Check for diagonal capture
    if (std::abs(target.x - currentPosition.x) == TILE_SIZE && target.y == currentPosition.y + direction * TILE_SIZE)
    {
        return isOpponentPiece(target, pieces, getColor());
    }

    return false;
}

// Highlight valid moves for the HellPawn on the provided board
void HellPawn::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

    // Highlight single step forward
    sf::Vector2f singleStep = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
    if (singleStep.y >= 0 && singleStep.y < BOARD_SIZE * TILE_SIZE && canMoveTo(singleStep, pieces))
    {
        board[static_cast<int>(singleStep.y / TILE_SIZE)][static_cast<int>(singleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight double step forward on first move
    sf::Vector2f doubleStep = sf::Vector2f(currentPosition.x, currentPosition.y + 2 * direction * TILE_SIZE);
    if (doubleStep.y >= 0 && doubleStep.y < BOARD_SIZE * TILE_SIZE &&
        ((getColor() == Piece::Color::White && currentPosition.y == 1 * TILE_SIZE) ||
         (getColor() == Piece::Color::Black && currentPosition.y == 6 * TILE_SIZE)) &&
        canMoveTo(doubleStep, pieces))
    {
        board[static_cast<int>(doubleStep.y / TILE_SIZE)][static_cast<int>(doubleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight diagonal captures
    sf::Vector2f diagonalLeft = sf::Vector2f(currentPosition.x - TILE_SIZE, currentPosition.y + direction * TILE_SIZE);
    if (diagonalLeft.x >= 0 && diagonalLeft.x < BOARD_SIZE * TILE_SIZE && diagonalLeft.y >= 0 && diagonalLeft.y < BOARD_SIZE * TILE_SIZE &&
        canMoveTo(diagonalLeft, pieces))
    {
        board[static_cast<int>(diagonalLeft.y / TILE_SIZE)][static_cast<int>(diagonalLeft.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
    }

    sf::Vector2f diagonalRight = sf::Vector2f(currentPosition.x + TILE_SIZE, currentPosition.y + direction * TILE_SIZE);
    if (diagonalRight.x >= 0 && diagonalRight.x < BOARD_SIZE * TILE_SIZE && diagonalRight.y >= 0 && diagonalRight.y < BOARD_SIZE * TILE_SIZE &&
        canMoveTo(diagonalRight, pieces))
    {
        board[static_cast<int>(diagonalRight.y / TILE_SIZE)][static_cast<int>(diagonalRight.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
    }
}
void HellPawn::infect(const Piece *capturedPiece, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board, TextureManager &textureManager)
{
    // Check if the captured piece is not a 'pawn' type
    if (dynamic_cast<const Pawn *>(capturedPiece) == nullptr)
    {
        // Get the type of the captured piece
        std::string pieceType = capturedPiece->getType();
        std::cout << "Converting piece of type: " << pieceType << std::endl;

        // Determine the new piece's texture
        std::string textureName = (getColor() == Piece::Color::White ? "White" : "Black") + pieceType;
        sf::Texture *newTexture = textureManager.getTexture(textureName);

        if (!newTexture)
        {
            std::cerr << "Failed to get texture for: " << textureName << std::endl;
            return;
        }

        // Create a new piece with the same type and updated texture
        auto it = std::find_if(pieces.begin(), pieces.end(), [&](const std::unique_ptr<Piece> &piece)
                               { return piece.get() == capturedPiece; });

        if (it != pieces.end())
        {
            // Get the position and color of the captured piece
            sf::Vector2f position = capturedPiece->getPosition();
            Piece::Color color = this->getColor();

            // Create a new piece of the same type as the captured piece
            int id = getNextID();
            std::unique_ptr<Piece> newPiece = createPiece(id, pieceType, textureManager, position, color);

            if (newPiece)
            {
                newPiece->setTexture(*newTexture);
                newPiece->setColor(color);

                // Replace the captured piece with the new piece in the vector
                (*it) = std::move(newPiece);

                // Erase the HellPawn that performed the capturing
                toBeRemoved = true;
                std::cout << "Converted and replaced piece of type: " << pieceType << std::endl;
            }
            else
            {
                std::cerr << "Failed to create new piece for type: " << pieceType << std::endl;
            }
        }
        else
        {
            std::cerr << "Captured piece not found in pieces vector!" << std::endl;
        }
    }
    else
    {
        // Capture a pawn regularly
        this->capture(capturedPiece->getPosition(), pieces);
    }
}
