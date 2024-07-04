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

// Special Features or Notes:
// - The HellPawn is a Level 2 Pawn piece belonging to the Hellspawn race.
// - The HellPawn has the movement and capture abilities of a pawn.
//   It moves forward one square but has the option to move two squares on its first move.
// - It captures diagonally one square ahead, under specific conditions.
// - HellPawn has the special capture ability, that when it captures a non-pawn piece,
//   it becomes a copy of that piece in its own allied color.
// - Currently does not contain logic for a Pawn promotion.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the HellPawn piece.

#include "hellPawn.h"
#include "queen.h"
#include "rook.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "wizard.h"
#include "necromancer.h"
#include "beastKnight.h"
#include "boulderThrower.h"
#include "howler.h"
#include "prowler.h"
#include "utility.h"
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

void HellPawn::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    auto targetPieceIt = pieces.end();

    // Find the target piece and check if it's an opponent piece
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        if ((*it)->getPosition() == target && isOpponentPiece(target, pieces, getColor()))
        {
            targetPieceIt = it;
            break;
        }
    }

    // If an opponent piece is found, change its color and texture if it's a Rook
    if (targetPieceIt != pieces.end())
    {
        if ((*targetPieceIt)->getType() == "Rook")
        {
            // Cast to Rook and change its color and texture
            Rook *rook = dynamic_cast<Rook *>(targetPieceIt->get());
            if (rook)
            {
                rook->setColor(this->getColor());
                sf::Texture &newTexture = getRookTexture();
                rook->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Knight")
        {
            // Cast to Rook and change its color and texture
            Knight *knight = dynamic_cast<Knight *>(targetPieceIt->get());
            if (knight)
            {
                knight->setColor(this->getColor());
                sf::Texture &newTexture = getKnightTexture();
                knight->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Bishop")
        {
            // Cast to Rook and change its color and texture
            Bishop *bishop = dynamic_cast<Bishop *>(targetPieceIt->get());
            if (bishop)
            {
                bishop->setColor(this->getColor());
                sf::Texture &newTexture = getBishopTexture();
                bishop->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Queen")
        {
            // Cast to Queen and change its color and texture
            Queen *queen = dynamic_cast<Queen *>(targetPieceIt->get());
            if (queen)
            {
                queen->setColor(this->getColor());
                sf::Texture &newTexture = getQueenTexture();
                queen->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "BeastKnight")
        {
            // Cast to BeastKnight and change its color and texture
            BeastKnight *beastKnight = dynamic_cast<BeastKnight *>(targetPieceIt->get());
            if (beastKnight)
            {
                beastKnight->setColor(this->getColor());
                sf::Texture &newTexture = getBeastKnightTexture();
                beastKnight->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "BoulderThrower")
        {
            // Cast to BoulderThrower and change its color and texture
            BoulderThrower *boulderThrower = dynamic_cast<BoulderThrower *>(targetPieceIt->get());
            if (boulderThrower)
            {
                boulderThrower->setColor(this->getColor());
                sf::Texture &newTexture = getBoulderThrowerTexture();
                boulderThrower->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Howler")
        {
            // Cast to Howler and change its color and texture
            Howler *howler = dynamic_cast<Howler *>(targetPieceIt->get());
            if (howler)
            {
                howler->setColor(this->getColor());
                sf::Texture &newTexture = getHowlerTexture();
                howler->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Prowler")
        {
            // Cast to Prowler and change its color and texture
            Prowler *prowler = dynamic_cast<Prowler *>(targetPieceIt->get());
            if (prowler)
            {
                prowler->setColor(this->getColor());
                sf::Texture &newTexture = getProwlerTexture();
                prowler->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Necromancer")
        {
            // Cast to Necromancer and change its color and texture
            Necromancer *necromancer = dynamic_cast<Necromancer *>(targetPieceIt->get());
            if (necromancer)
            {
                necromancer->setColor(this->getColor());
                sf::Texture &newTexture = getNecromancerTexture();
                necromancer->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else if ((*targetPieceIt)->getType() == "Wizard")
        {
            // Cast to wizard and change its color and texture
            Wizard *wizard = dynamic_cast<Wizard *>(targetPieceIt->get());
            if (wizard)
            {
                wizard->setColor(this->getColor());
                sf::Texture &newTexture = getWizardTexture();
                wizard->setTexture(newTexture);
                // Mark this HellPawn instance for removal after the capture operation
                toBeRemoved = true;
            }
        }

        else
        {
            // Regular capture
            auto it = std::remove_if(pieces.begin(), pieces.end(),
                                     [&](const std::unique_ptr<Piece> &piece)
                                     {
                                         return piece->getPosition() == target && piece->getColor() != getColor();
                                     });
            pieces.erase(it, pieces.end());
            setPosition(target);
        }
    }
    else
    {
        std::cerr << "No opponent piece found at the target position!" << std::endl;
    }
}

sf::Texture &HellPawn::getRookTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteRookTexture : blackRookTexture;
}

sf::Texture &HellPawn::getKnightTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteKnightTexture : blackKnightTexture;
}

sf::Texture &HellPawn::getBishopTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteBishopTexture : blackBishopTexture;
}

sf::Texture &HellPawn::getQueenTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteQueenTexture : blackQueenTexture;
}

sf::Texture &HellPawn::getBeastKnightTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteBeastKnightTexture : blackBeastKnightTexture;
}

sf::Texture &HellPawn::getBoulderThrowerTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteBoulderThrowerTexture : blackBoulderThrowerTexture;
}

sf::Texture &HellPawn::getHowlerTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteHellPawnTexture : blackHellPawnTexture;
}

sf::Texture &HellPawn::getProwlerTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteProwlerTexture : blackProwlerTexture;
}

sf::Texture &HellPawn::getWizardTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteWizardTexture : blackWizardTexture;
}

sf::Texture &HellPawn::getNecromancerTexture() const
{
    return (getColor() == Piece::Color::White) ? whiteNecromancerTexture : blackNecromancerTexture;
}