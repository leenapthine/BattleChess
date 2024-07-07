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

    // If an opponent piece is found, change its color and texture if it's a known piece type
    if (targetPieceIt != pieces.end())
    {
        Piece *targetPiece = targetPieceIt->get();
        bool isKnownPieceType = true;

        std::string pieceType = targetPiece->getType();
        std::cout << "Captured piece type: " << pieceType << std::endl;

        if (pieceType == "Rook")
        {
            Rook *rook = dynamic_cast<Rook *>(targetPiece);
            if (rook)
            {
                rook->setColor(this->getColor());
                sf::Texture &newTexture = getRookTexture();
                rook->setTexture(newTexture);
                std::cout << "Assigned Rook texture" << std::endl;
            }
        }
        else if (pieceType == "Knight")
        {
            Knight *knight = dynamic_cast<Knight *>(targetPiece);
            if (knight)
            {
                knight->setColor(this->getColor());
                sf::Texture &newTexture = getKnightTexture();
                knight->setTexture(newTexture);
                std::cout << "Assigned Knight texture" << std::endl;
            }
        }
        else if (pieceType == "Bishop")
        {
            Bishop *bishop = dynamic_cast<Bishop *>(targetPiece);
            if (bishop)
            {
                bishop->setColor(this->getColor());
                sf::Texture &newTexture = getBishopTexture();
                bishop->setTexture(newTexture);
                std::cout << "Assigned Bishop texture" << std::endl;
            }
        }
        else if (pieceType == "Queen")
        {
            Queen *queen = dynamic_cast<Queen *>(targetPiece);
            if (queen)
            {
                queen->setColor(this->getColor());
                sf::Texture &newTexture = getQueenTexture();
                queen->setTexture(newTexture);
                std::cout << "Assigned Queen texture" << std::endl;
            }
        }
        else if (pieceType == "BeastKnight")
        {
            BeastKnight *beastKnight = dynamic_cast<BeastKnight *>(targetPiece);
            if (beastKnight)
            {
                beastKnight->setColor(this->getColor());
                sf::Texture &newTexture = getBeastKnightTexture();
                beastKnight->setTexture(newTexture);
                std::cout << "Assigned BeastKnight texture" << std::endl;
            }
        }
        else if (pieceType == "BoulderThrower")
        {
            BoulderThrower *boulderThrower = dynamic_cast<BoulderThrower *>(targetPiece);
            if (boulderThrower)
            {
                boulderThrower->setColor(this->getColor());
                sf::Texture &newTexture = getBoulderThrowerTexture();
                boulderThrower->setTexture(newTexture);
                std::cout << "Assigned BoulderThrower texture" << std::endl;
            }
        }
        else if (pieceType == "Howler")
        {
            Howler *howler = dynamic_cast<Howler *>(targetPiece);
            if (howler)
            {
                howler->setColor(this->getColor());
                sf::Texture &newTexture = getHowlerTexture();
                howler->setTexture(newTexture);
                std::cout << "Assigned Howler texture" << std::endl;
            }
        }
        else if (pieceType == "Prowler")
        {
            Prowler *prowler = dynamic_cast<Prowler *>(targetPiece);
            if (prowler)
            {
                prowler->setColor(this->getColor());
                sf::Texture &newTexture = getProwlerTexture();
                prowler->setTexture(newTexture);
                std::cout << "Assigned Prowler texture" << std::endl;
            }
        }
        else if (pieceType == "Necromancer")
        {
            Necromancer *necromancer = dynamic_cast<Necromancer *>(targetPiece);
            if (necromancer)
            {
                necromancer->setColor(this->getColor());
                sf::Texture &newTexture = getNecromancerTexture();
                necromancer->setTexture(newTexture);
                std::cout << "Assigned Necromancer texture" << std::endl;
            }
        }
        else if (pieceType == "Wizard")
        {
            Wizard *wizard = dynamic_cast<Wizard *>(targetPiece);
            if (wizard)
            {
                wizard->setColor(this->getColor());
                sf::Texture &newTexture = getWizardTexture();
                wizard->setTexture(newTexture);
                std::cout << "Assigned Wizard texture" << std::endl;
            }
        }
        else
        {
            isKnownPieceType = false;
            // Regular capture
            auto it = std::remove_if(pieces.begin(), pieces.end(),
                                     [&](const std::unique_ptr<Piece> &piece)
                                     {
                                         return piece->getPosition() == target && piece->getColor() != getColor();
                                     });
            pieces.erase(it, pieces.end());
            setPosition(target);
            std::cout << "Captured and removed regular piece" << std::endl;
        }

        // Mark this HellPawn instance for removal after the capture operation
        if (isKnownPieceType)
        {
            toBeRemoved = true;
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
    return (getColor() == Piece::Color::White) ? whiteHowlerTexture : blackHowlerTexture;
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