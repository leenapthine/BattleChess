// Filename: howler.cpp
// Description: Implementation of the Howler class, a versatile chess piece that starts with bishop-like abilities and gains new movement abilities from captured pieces.

// Main Classes:
// - Howler: Manages the movement and capture behavior of the Howler piece, including dynamic movement ability acquisition from captured pieces.

// Main Functions:
// - void Howler::gainInitialAbilities(): Sets the initial abilities of the Howler piece to those of a Bishop.
// - bool Howler::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const: Determines if the Howler can move to a given target position based on its current abilities.
// - void Howler::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const: Highlights all valid moves for the Howler on the provided chessboard, considering its current abilities.
// - void Howler::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces): Executes the capture of an opponent piece located at the specified target position and gains the movment abilities of the captured piece.
// - void Howler::gainBishopAbilities(): Grants the Howler the movement and highlighting abilities of a Bishop.
// - void Howler::gainRookAbilities(): Grants the Howler the movement and highlighting abilities of a Rook.
// - void Howler::gainKnightAbilities(): Grants the Howler the movement and highlighting abilities of a Knight.
// - etc.
// - void Howler::gainAbilitiesFromCapturedPiece(const Piece &capturedPiece): Grants the Howler new abilities based on the type of the captured piece.

// Special Features or Notes:
// - The Howler starts with Bishop abilities and can acquire movement abilities from other pieces it captures.
// - The Howler's abilities are dynamic and can change throughout the game based on the pieces it captures.
// - The Howler will only ever acquire the movement from the Level 1 piece type. Ex. If a Necromancer is captured,
//   the Howler will aquire the movement and capture abilities of the Bishop.
// - Current Howler logic has the code to implement full movement and capture of higher level Pieces in case we choose to use this
//   flexibility at a later stage in development.

// Usage or Context:
// - This file implements the dynamic behavior of the Howler piece in a our Battle Chess game.

#include "howler.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include "bishop.h"
#include "rook.h"
#include "pawn.h"
#include "knight.h"
#include "queen.h"
#include "king.h"
#include "necromancer.h"
#include "necroPawn.h"
#include "beastKnight.h"
#include "boulderThrower.h"
#include "wizard.h"
#include <iostream>

// Initialize abilities, Howler starts with Bishop abilities
void Howler::gainInitialAbilities()
{
    gainBishopAbilities();
}

// Check if Howler can move to the target position based on current abilities
bool Howler::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    for (const auto &ability : movementAbilities)
    {
        if (ability(target, pieces))
        {
            return true;
        }
    }
    return false;
}

// Highlight all valid moves for Howler on the board based on current abilities
void Howler::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    for (const auto &highlight : highlightAbilities)
    {
        highlight(board, pieces);
    }
}

// Capture an opponent piece at the target position and gain its abilities
void Howler::capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
{
    std::cout << "Howler attempting to capture at: (" << target.x / TILE_SIZE << ", " << target.y / TILE_SIZE << ")" << std::endl;

    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        if ((*it)->getPosition() == target && isOpponentPiece(target, pieces, getColor()))
        {
            std::cout << "Howler captured piece: " << (*it)->getType() << std::endl;
            gainAbilitiesFromCapturedPiece(**it);
            pieces.erase(it);
            break;
        }
    }
    setPosition(target);
}

// Grant Howler the movement and highlighting abilities of a Bishop
void Howler::gainBishopAbilities()
{
    if (bishopAbilities)
        return; // Prevent adding the same ability multiple times
    bishopAbilities = true;

    movementAbilities.push_back([this](const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) -> bool
                                {
        sf::Vector2f currentPosition = getPosition();

        // Bishop can only move diagonally
        if (std::abs(target.x - currentPosition.x) != std::abs(target.y - currentPosition.y))
        {
            return false;
        }

        // Check if the path is clear
        return isPathClear(currentPosition, target, pieces); });

    highlightAbilities.push_back([this](std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) -> void
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
        } });
}

// Grant Howler the movement and highlighting abilities of a Rook
void Howler::gainRookAbilities()
{
    if (rookAbilities)
        return; // Prevent adding the same ability multiple times
    rookAbilities = true;

    std::cout << "Howler gained Rook abilities." << std::endl;
    movementAbilities.push_back([this](const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) -> bool
                                {
        sf::Vector2f currentPosition = getPosition();

        // Rook can only move in straight lines
        if (target.x != getPosition().x && target.y != getPosition().y)
        {
            return false;
        }

        // Check if the path is clear
        if (!isPathClear(currentPosition, target, pieces))
        {
            return false;
        }

        // Check if the target position is occupied by an opponent's piece
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor()); });

    highlightAbilities.push_back([this](std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) -> void
                                 {
        sf::Vector2f currentPosition = getPosition();

        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            sf::Vector2f horizontalTarget(i * TILE_SIZE, currentPosition.y);
            if (canMoveTo(horizontalTarget, pieces))
            {
                if (!isOpponentPiece(horizontalTarget, pieces, getColor()))
                {
                    board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true);
                }
                else if (isOpponentPiece(horizontalTarget, pieces, getColor()))
                {
                    board[static_cast<int>(currentPosition.y / TILE_SIZE)][i].setHighlight(true, sf::Color::Red);
                }
            }

            sf::Vector2f verticalTarget(currentPosition.x, i * TILE_SIZE);
            if (canMoveTo(verticalTarget, pieces))
            {
                if (!isOpponentPiece(verticalTarget, pieces, getColor()))
                {
                    board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true);
                }
                else if (isOpponentPiece(verticalTarget, pieces, getColor()))
                {
                    board[i][static_cast<int>(currentPosition.x / TILE_SIZE)].setHighlight(true, sf::Color::Red);
                }
            }
        } });
}

// Grant Howler the movement and highlighting abilities of a Knight
void Howler::gainKnightAbilities()
{
    if (knightAbilities)
        return; // Prevent adding the same ability multiple times
    knightAbilities = true;

    std::cout << "Howler gained Knight abilities." << std::endl;
    movementAbilities.push_back([this](const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) -> bool
                                {
        sf::Vector2f currentPosition = getPosition();
        int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
        int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
        }
        return false; });

    highlightAbilities.push_back([this](std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) -> void
                                 {
        sf::Vector2f currentPosition = getPosition();
        std::vector<sf::Vector2f> moves = {
            {currentPosition.x + 2 * TILE_SIZE, currentPosition.y + TILE_SIZE},
            {currentPosition.x + 2 * TILE_SIZE, currentPosition.y - TILE_SIZE},
            {currentPosition.x - 2 * TILE_SIZE, currentPosition.y + TILE_SIZE},
            {currentPosition.x - 2 * TILE_SIZE, currentPosition.y - TILE_SIZE},
            {currentPosition.x + TILE_SIZE, currentPosition.y + 2 * TILE_SIZE},
            {currentPosition.x + TILE_SIZE, currentPosition.y - 2 * TILE_SIZE},
            {currentPosition.x - TILE_SIZE, currentPosition.y + 2 * TILE_SIZE},
            {currentPosition.x - TILE_SIZE, currentPosition.y - 2 * TILE_SIZE}};

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
        } });
}

// Grant Howler the movement and highlighting abilities of a Pawn
void Howler::gainPawnAbilities()
{
    if (pawnAbilities)
        return; // Prevent adding the same ability multiple times
    pawnAbilities = true;

    std::cout << "Howler gained Pawn abilities." << std::endl;
    movementAbilities.push_back([this](const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) -> bool
                                {
        sf::Vector2f currentPosition = getPosition();
        float direction = (getColor() == Piece::Color::White) ? 1.0f : -1.0f; // White moves down, Black moves up

        // Ensure target is within the board bounds
        if (target.x < 0 || target.x >= BOARD_SIZE * TILE_SIZE || target.y < 0 || target.y >= BOARD_SIZE * TILE_SIZE)
        {
            std::cout << "Target out of bounds: (" << target.x << ", " << target.y << ")\n";
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
        return false; });

    highlightAbilities.push_back([this](std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) -> void
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
        } });
}

// Grant Howler the movement and highlighting abilities of a Queen
void Howler::gainQueenAbilities()
{
    if (queenAbilities)
        return; // Prevent adding the same ability multiple times
    queenAbilities = true;

    std::cout << "Howler gained Queen abilities." << std::endl;
    movementAbilities.push_back([this](const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) -> bool
                                {

        sf::Vector2f currentPosition = getPosition();

        // Queen can only move in straight lines and diagnal lines
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
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor()); });

    highlightAbilities.push_back([this](std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) -> void
                                 {

        sf::Vector2f currentPosition = getPosition();
        Piece::Color currentPlayerColor = getColor();

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
        } });
}

// Grant Howler the movement and highlighting abilities of a King
void Howler::gainKingAbilities()
{
    if (kingAbilities)
        return; // Prevent adding the same ability multiple times
    kingAbilities = true;

    std::cout << "Howler gained King abilities." << std::endl;
    movementAbilities.push_back([this](const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) -> bool
                                {
        sf::Vector2f currentPosition = getPosition();
        int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
        int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

        if ((dx == 0 && dy == 1) || (dx == 0 && dy == -1) || (dx == 1 && dy == 0) || (dx == 1 && dy == -1) || (dx == 1 && dy == 1) || (dx == -1 && dy == 0) || (dx == -1 && dy == 1) || (dx == -1 && dy == -1))
        {
            return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
        }
        return false; });

    highlightAbilities.push_back([this](std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) -> void
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
        } });
}

// Grant Howler abilities based on the captured piece type
void Howler::gainAbilitiesFromCapturedPiece(const Piece &capturedPiece)
{
    std::cout << "Howler is gaining abilities from captured piece: " << capturedPiece.getType() << std::endl;

    std::string capturedType = capturedPiece.getType();

    // Define sets of types for each category
    const std::unordered_set<std::string> rookTypes = {"Rook", "BoulderThrower", "DeadLauncher", "Portal", "Beholder"};
    const std::unordered_set<std::string> knightTypes = {"Knight", "BeastKnight", "Prowler", "Familiar", "GhostKnight"};
    const std::unordered_set<std::string> queenTypes = {"Queen", "QueenOfIllusions", "QueenOfDomination", "QueenOfDestruction", "QueenOfBones"};
    const std::unordered_set<std::string> kingTypes = {"King", "WizardKing", "GhoulKing", "HellKing", "FrogKing"};
    const std::unordered_set<std::string> pawnTypes = {"Pawn", "NecroPawn", "YoungWiz", "PawnHopper", "HellPawn"};
    const std::unordered_set<std::string> bishopTypes = {"Bishop", "Wizard", "Beholder", "BeastDruid", "Necromancer"};

    // Check the captured piece type and gain corresponding abilities
    if (rookTypes.find(capturedType) != rookTypes.end())
    {
        gainRookAbilities();
    }
    else if (knightTypes.find(capturedType) != knightTypes.end())
    {
        gainKnightAbilities();
    }
    else if (queenTypes.find(capturedType) != queenTypes.end())
    {
        gainQueenAbilities();
    }
    else if (kingTypes.find(capturedType) != kingTypes.end())
    {
        gainKingAbilities();
    }
    else if (pawnTypes.find(capturedType) != pawnTypes.end())
    {
        std::cout << "Howler gains: " << capturedPiece.getType() << " abilities." << std::endl;
        gainPawnAbilities();
    }
    else if (bishopTypes.find(capturedType) != bishopTypes.end())
    {
        gainBishopAbilities();
    }
};
