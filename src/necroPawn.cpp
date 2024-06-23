// Filename: necroPawn.cpp
// Description: Implementation of the NecroPawn class, a special pawn piece with unique abilities such as sacrificial captures.

// Main Classes:
// - NecroPawn: Manages the movement and special abilities (sacrifice) of the NecroPawn piece.

// Main Functions:
// - bool NecroPawn::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Determines if the NecroPawn can move to a given target position based on its movement rules.
// - void NecroPawn::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the NecroPawn on the provided chessboard.
// - void NecroPawn::sacrifice(std::vector<std::unique_ptr<Piece>> &pieces):
//   Sacrifices the NecroPawn to capture adjacent/diagnal (both friendly and opponent) pieces and remove itself from the board.
// - std::vector<sf::Vector2f> NecroPawn::getCapturePositions() const: Returns all adjacent/diagnal positions for potential captures.

// Special Features or Notes:
// - NecroPawn moves like a regular pawn. It is a Level 2 Pawn piece belonging to the Necromancer race.
// - NecroPawn can either capture diagnally like a regular pawn or,
//   It has a unique ability to sacrifice itself to capture all adjacent and diagnal pieces.
// - It inherits from the Piece class.

// Usage or Context:
// - This file implements the behavior and unique abilities of the NecroPawn piece in our Battle Chess game.

#include "necroPawn.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream> // Include for debug output

// Determine if the Pawn can move to a specified target position
bool NecroPawn::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

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
            // Ensure both intermediate and target positions are not occupied
            sf::Vector2f intermediatePosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
            return !getPieceAtPosition(target, pieces) && !getPieceAtPosition(intermediatePosition, pieces);
        }
    }

    // Check for diagonal capture
    if (std::abs(target.x - currentPosition.x) == TILE_SIZE && target.y == currentPosition.y + direction * TILE_SIZE)
    {
        // Ensure the target position is occupied by an opponent's piece
        return isOpponentPiece(target, pieces, getColor());
    }

    return false;
}

// Highlight valid moves for the Pawn on the provided board
void NecroPawn::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int currentRow = currentPosition.y / TILE_SIZE;
    int currentCol = currentPosition.x / TILE_SIZE;
    float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

    // Highlight the current position in red
    board[currentRow][currentCol].setHighlight(true, sf::Color::Red);

    // Highlight single step forward
    sf::Vector2f singleStep = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
    if (canMoveTo(singleStep, pieces))
    {
        board[static_cast<int>(singleStep.y / TILE_SIZE)][static_cast<int>(singleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight double step forward on first move
    sf::Vector2f doubleStep = sf::Vector2f(currentPosition.x, currentPosition.y + 2 * direction * TILE_SIZE);
    if (((getColor() == Piece::Color::White && currentPosition.y == 1 * TILE_SIZE) ||
         (getColor() == Piece::Color::Black && currentPosition.y == 6 * TILE_SIZE)) &&
        canMoveTo(doubleStep, pieces))
    {
        board[static_cast<int>(doubleStep.y / TILE_SIZE)][static_cast<int>(doubleStep.x / TILE_SIZE)].setHighlight(true);
    }

    // Highlight diagonal captures in red
    sf::Vector2f diagonalLeft = sf::Vector2f(currentPosition.x - TILE_SIZE, currentPosition.y + direction * TILE_SIZE);
    if (canMoveTo(diagonalLeft, pieces))
    {
        board[static_cast<int>(diagonalLeft.y / TILE_SIZE)][static_cast<int>(diagonalLeft.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
    }

    sf::Vector2f diagonalRight = sf::Vector2f(currentPosition.x + TILE_SIZE, currentPosition.y + direction * TILE_SIZE);
    if (canMoveTo(diagonalRight, pieces))
    {
        board[static_cast<int>(diagonalRight.y / TILE_SIZE)][static_cast<int>(diagonalRight.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
    }
}

// The sacrifice ability captures itself and all adjacent and vertical pieces.
void NecroPawn::sacrifice(std::vector<std::unique_ptr<Piece>> &pieces)
{
    sf::Vector2f currentPosition = getPosition();
    std::vector<sf::Vector2f> capturePositions = getCapturePositions();

    // Iterate over all adjacent positions and capture any pieces found
    for (const auto &pos : capturePositions)
    {
        for (auto it = pieces.begin(); it != pieces.end();)
        {
            if ((*it)->getPosition() == pos)
            {
                std::cout << "Capturing piece at position: (" << pos.x / TILE_SIZE << ", " << pos.y / TILE_SIZE << ")\n";
                it = pieces.erase(it); // Remove the piece and update iterator
            }
            else
            {
                ++it;
            }
        }
    }

    // Sacrifice this NecroPawn by removing it from the pieces
    for (auto it = pieces.begin(); it != pieces.end();)
    {
        if ((*it)->getPosition() == currentPosition)
        {
            std::cout << "NecroPawn at (" << currentPosition.x / TILE_SIZE << ", " << currentPosition.y / TILE_SIZE << ") has been sacrificed.\n";
            it = pieces.erase(it); // Remove the NecroPawn and update iterator
        }
        else
        {
            ++it;
        }
    }
}

// Returns all adjacent and vertical squares to the current selected position
std::vector<sf::Vector2f> NecroPawn::getCapturePositions() const
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
}