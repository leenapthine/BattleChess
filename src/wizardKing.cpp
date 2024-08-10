// Filename: WizardKing.cpp
// Description: Implementation of the WizardKing class, which defines the behavior and movement rules of the FrogKing piece in our chess game.

// Main Classes:
// - WizardKing: Represents the WizardKing piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool WizardKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the WizardKing can legally move to the specified target position on the chessboard.
// - void WizardKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the WizardKing on the provided chessboard, considering its current position and game rules.
// - void WizardKing::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
//   Highlights all enemies in a vertical line of sight from the WizardKing.
// - void WizardKing::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
//   Unique capture method. Captures regular like a King Piece, but can also capture from a distance using vertical line of sight.
// - std::vector<sf::Vector2f> WizardKing::getCapturePositions() const
//   Determines all squares in the perimeter.

// Special Features or Notes:
// - WizardKing is level 2 King piece belonging to the Wizard guild.
// - The WizardKing moves one square in any direction.
// - It captures on movement into a space occupied by an opponent piece, but also from a distance
//   using vertical line of sight.
// - Currently does not contain logic for the King / Rook 'castling' function.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the WizardKing piece.

#include "piece.h"
#include "wizardKing.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the WizardKing can move to a specified target position
bool WizardKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 0 && dy == 1) || (dx == 0 && dy == -1) || (dx == 1 && dy == 0) || (dx == 1 && dy == -1) || (dx == 1 && dy == 1) || (dx == -1 && dy == 0) || (dx == -1 && dy == 1) || (dx == -1 && dy == -1))
    {
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false;
}

// Highlight valid moves for the WizardKing on the provided board
void WizardKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    std::vector<sf::Vector2f> moves = {
        {currentPosition.x, currentPosition.y + TILE_SIZE},
        {currentPosition.x, currentPosition.y - TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y},
        {currentPosition.x - TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y},
        {currentPosition.x + TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y - TILE_SIZE}};

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
}

void WizardKing::highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int startX = static_cast<int>(currentPosition.x / TILE_SIZE);
    int startY = static_cast<int>(currentPosition.y / TILE_SIZE);

    // Define a lambda to check if a position is within bounds and should be highlighted
    auto highlightPosition = [&](int col, int row, sf::Color color)
    {
        if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
        {
            board[row][col].setHighlight(true, color);
        }
    };

    // Check vertical movement upwards
    for (int row = startY + 1; row < BOARD_SIZE; ++row)
    {
        sf::Vector2f target(startX * TILE_SIZE, row * TILE_SIZE);

        // Check if the path is clear
        if (isPathClear(currentPosition, target, pieces))
        {
            // Highlight if the target is an opponent piece
            if (isOpponentPiece(target, pieces, getColor()))
            {
                highlightPosition(startX, row, sf::Color::Red);
                break; // Stop after highlighting an opponent piece
            }
        }
    }

    // Check vertical movement downwards
    for (int row = startY - 1; row >= 0; --row)
    {
        sf::Vector2f target(startX * TILE_SIZE, row * TILE_SIZE);

        // Check if the path is clear
        if (isPathClear(currentPosition, target, pieces))
        {
            // Highlight if the target is an opponent piece
            if (isOpponentPiece(target, pieces, getColor()))
            {
                highlightPosition(startX, row, sf::Color::Red);
                break; // Stop after highlighting an opponent piece
            }
        }
    }
}

void WizardKing::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    // Get the perimeter positions where the WizardKing can capture
    std::vector<sf::Vector2f> perimeter = getCapturePositions();
    bool isInPerimeter = false;

    // Check if the target position is within the perimeter
    for (const auto &square : perimeter)
    {
        if (target == square)
        {
            isInPerimeter = true;
            break;
        }
    }

    // Common capture logic
    auto it = std::remove_if(pieces.begin(), pieces.end(),
                             [&](const std::unique_ptr<Piece> &piece)
                             {
                                 return piece->getPosition() == target && piece->getColor() != getColor();
                             });
    pieces.erase(it, pieces.end());

    // If in perimeter, move the WizardKing to the target position
    if (isInPerimeter)
    {
        setPosition(target);
    }
}

// Returns all adjacent and vertical squares to the current selected position
std::vector<sf::Vector2f> WizardKing::getCapturePositions() const
{
    sf::Vector2f currentPosition = getPosition();
    return {
        {currentPosition.x + TILE_SIZE, currentPosition.y},
        {currentPosition.x - TILE_SIZE, currentPosition.y},
        {currentPosition.x, currentPosition.y + TILE_SIZE},
        {currentPosition.x, currentPosition.y - TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x + TILE_SIZE, currentPosition.y - TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y + TILE_SIZE},
        {currentPosition.x - TILE_SIZE, currentPosition.y - TILE_SIZE}};
};