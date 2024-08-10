// Filename: utility.cpp
// Description: Implementation of various utility functions used throughout the chess game, including piece retrieval, path checking, input handling, and rendering.

// Main Functions:
// - bool isPathClear(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces):
//   Checks that a path is clear when moving a piece on the board.
//
// - void loadTextures(std::vector<std::unique_ptr<Piece>> &pieces):
//   Loads the textures for the pieces, scales thems, and then places them in their positions on the chess board.
//
// - void pushPawn(std::vector<std::unique_ptr<Piece>> &pieces, sf::Texture &pawnTexture, const sf::Vector2f &position, Piece::Color pieceColor):
//   Pushes a Pawn on the board at the target location.
//
// - Piece* getPieceAtPosition(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces):
//   Retrieves the piece located at the specified position on the board.
//
// - bool isOpponentPiece(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces, Piece::Color currentPlayerColor):
//   Determines if the piece at the specified position belongs to the opponent.
//
// - void drawChessboard(sf::RenderWindow &window, std::vector<std::vector<Square>> &board):
//   Renders the chessboard by drawing each square to the window.
//
// - sf::Vector2i getUserInput():
//   Captures the user's keyboard input to determine the movement direction.
//
// - sf::Vector2f calculateNextPosition(const sf::Vector2f &currentPosition, const sf::Vector2i &movement):
//   Calculates the next position on the board based on the current position and the movement direction.
//
// - void highlightButtonOnHover(sf::RenderWindow &window, Button &button, const sf::Color &normalColor, const sf::Color &hoverColor):
//   Changes the button's color when the mouse hovers over it to indicate it's being hovered over.

// Usage or Context:
// - These utility functions are used throughout the chess game to manage pieces, handle user interactions, and render the game state.

#include "utility.h"
#include "globals.h"
#include "piece.h"
#include "textureManager.h"
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
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @brief Checks if the path between the starting position and the ending position is clear of any pieces.
 *
 * @param start The starting position on the board as an sf::Vector2f.
 * @param end The ending position on the board as an sf::Vector2f.
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @return true if the path is clear.
 * @return false if any piece is blocking the path.
 */
bool isPathClear(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces)
{
    sf::Vector2f direction = end - start;
    sf::Vector2f unitDirection(direction.x == 0 ? 0 : direction.x / std::abs(direction.x), direction.y == 0 ? 0 : direction.y / std::abs(direction.y));
    sf::Vector2f currentPosition = start + unitDirection * 80.0f; // Move one tile in the direction

    while (currentPosition != end)
    {
        for (const auto &piece : pieces)
        {
            if (piece->getPosition() == currentPosition)
            {
                return false; // Path is blocked
            }
        }
        currentPosition += unitDirection * 80.0f;
    }

    return true; // Path is clear
}

/**
 * @brief Checks if the path between the starting position and the ending position is clear of any pieces.
 *
 * @param start The starting position on the board as an sf::Vector2f.
 * @param end The ending position on the board as an sf::Vector2f.
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @return true if the path is clear.
 * @return false if any piece is blocking the path.
 */
void loadTextures(TextureManager &textureManager)
{
    // Load textures
    textureManager.loadTexture("WhiteRook", "../resources/WhiteRook.png");
    textureManager.loadTexture("BlackRook", "../resources/BlackRook.png");
    textureManager.loadTexture("WhiteBishop", "../resources/WhiteBishop.png");
    textureManager.loadTexture("BlackBishop", "../resources/BlackBishop.png");
    textureManager.loadTexture("WhitePawn", "../resources/WhitePawn.png");
    textureManager.loadTexture("BlackPawn", "../resources/BlackPawn.png");
    textureManager.loadTexture("WhiteKnight", "../resources/WhiteKnight.png");
    textureManager.loadTexture("BlackKnight", "../resources/BlackKnight.png");
    textureManager.loadTexture("WhiteKing", "../resources/WhiteKing.png");
    textureManager.loadTexture("BlackKing", "../resources/BlackKing.png");
    textureManager.loadTexture("WhiteQueen", "../resources/WhiteQueen.png");
    textureManager.loadTexture("BlackQueen", "../resources/BlackQueen.png");
    textureManager.loadTexture("WhiteWizard", "../resources/WhiteWizard.png");
    textureManager.loadTexture("BlackWizard", "../resources/BlackWizard.png");
    textureManager.loadTexture("WhiteNecromancer", "../resources/WhiteNecromancer.png");
    textureManager.loadTexture("BlackNecromancer", "../resources/BlackNecromancer.png");
    textureManager.loadTexture("WhiteNecroPawn", "../resources/WhiteNecroPawn.png");
    textureManager.loadTexture("BlackNecroPawn", "../resources/BlackNecroPawn.png");
    textureManager.loadTexture("WhiteBeastKnight", "../resources/WhiteBeastKnight.png");
    textureManager.loadTexture("BlackBeastKnight", "../resources/BlackBeastKnight.png");
    textureManager.loadTexture("WhiteBoulderThrower", "../resources/WhiteBoulderThrower.png");
    textureManager.loadTexture("BlackBoulderThrower", "../resources/BlackBoulderThrower.png");
    textureManager.loadTexture("WhiteHowler", "../resources/WhiteHowler.png");
    textureManager.loadTexture("BlackHowler", "../resources/BlackHowler.png");
    textureManager.loadTexture("WhiteProwler", "../resources/WhiteProwler.png");
    textureManager.loadTexture("BlackProwler", "../resources/BlackProwler.png");
    textureManager.loadTexture("WhiteHellPawn", "../resources/WhiteHellPawn.png");
    textureManager.loadTexture("BlackHellPawn", "../resources/BlackHellPawn.png");
    textureManager.loadTexture("WhiteYoungWiz", "../resources/WhiteYoungWiz.png");
    textureManager.loadTexture("BlackYoungWiz", "../resources/BlackYoungWiz.png");
    textureManager.loadTexture("WhitePawnHopper", "../resources/WhitePawnHopper.png");
    textureManager.loadTexture("BlackPawnHopper", "../resources/BlackPawnHopper.png");
    textureManager.loadTexture("WhiteBeastDruid", "../resources/WhiteBeastDruid.png");
    textureManager.loadTexture("BlackBeastDruid", "../resources/BlackBeastDruid.png");
    textureManager.loadTexture("WhiteGhostKnight", "../resources/WhiteGhostKnight.png");
    textureManager.loadTexture("BlackGhostKnight", "../resources/BlackGhostKnight.png");
    textureManager.loadTexture("WhiteFamiliar", "../resources/WhiteFamiliar.png");
    textureManager.loadTexture("BlackFamiliar", "../resources/BlackFamiliar.png");
    textureManager.loadTexture("WhiteDeadLauncher", "../resources/WhiteDeadLauncher.png");
    textureManager.loadTexture("BlackDeadLauncher", "../resources/BlackDeadLauncher.png");
    textureManager.loadTexture("WhitePortal", "../resources/WhitePortal.png");
    textureManager.loadTexture("BlackPortal", "../resources/BlackPortal.png");
    textureManager.loadTexture("WhiteBeholder", "../resources/WhiteBeholder.png");
    textureManager.loadTexture("BlackBeholder", "../resources/BlackBeholder.png");
    textureManager.loadTexture("WhiteQueenOfIllusions", "../resources/WhiteQueenOfIllusions.png");
    textureManager.loadTexture("BlackQueenOfIllusions", "../resources/BlackQueenOfIllusions.png");
    textureManager.loadTexture("WhiteQueenOfBones", "../resources/WhiteQueenOfBones.png");
    textureManager.loadTexture("BlackQueenOfBones", "../resources/BlackQueenOfBones.png");
    textureManager.loadTexture("WhiteQueenOfDomination", "../resources/WhiteQueenOfDomination.png");
    textureManager.loadTexture("BlackQueenOfDomination", "../resources/BlackQueenOfDomination.png");
    textureManager.loadTexture("WhiteQueenOfDestruction", "../resources/WhiteQueenOfDestruction.png");
    textureManager.loadTexture("BlackQueenOfDestruction", "../resources/BlackQueenOfDestruction.png");
    textureManager.loadTexture("WhiteGhoulKing", "../resources/WhiteGhoulKing.png");
    textureManager.loadTexture("BlackGhoulKing", "../resources/BlackGhoulKing.png");
    textureManager.loadTexture("WhiteWizardKing", "../resources/WhiteWizardKing.png");
    textureManager.loadTexture("BlackWizardKing", "../resources/BlackWizardKing.png");
    textureManager.loadTexture("WhiteFrogKing", "../resources/WhiteFrogKing.png");
    textureManager.loadTexture("BlackFrogKing", "../resources/BlackFrogKing.png");
    textureManager.loadTexture("WhiteHellKing", "../resources/WhiteHellKing.png");
    textureManager.loadTexture("BlackHellKing", "../resources/BlackHellKing.png");
}

/**
 * @brief Checks if the path between the starting position and the ending position is clear of any pieces.
 *
 * @param start The starting position on the board as an sf::Vector2f.
 * @param end The ending position on the board as an sf::Vector2f.
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @return true if the path is clear.
 * @return false if any piece is blocking the path.
 */

void createPieces(TextureManager &textureManager, std::vector<std::unique_ptr<Piece>> &pieces)
{
    // Clear the pieces vector before adding new pieces
    pieces.clear();

    // Add new pieces to the pieces vector
    pieces.push_back(std::make_unique<DeadLauncher>(*textureManager.getTexture("WhiteDeadLauncher"), sf::Vector2f(0, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Portal>(*textureManager.getTexture("BlackPortal"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 1), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Portal>(*textureManager.getTexture("BlackPortal"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 1), TILE_SIZE * (BOARD_SIZE - 5)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Beholder>(*textureManager.getTexture("WhiteBeholder"), sf::Vector2f(560, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<BoulderThrower>(*textureManager.getTexture("BlackBoulderThrower"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 8), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Howler>(*textureManager.getTexture("WhiteHowler"), sf::Vector2f(160, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<BeastDruid>(*textureManager.getTexture("BlackBeastDruid"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 3), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Wizard>(*textureManager.getTexture("WhiteWizard"), sf::Vector2f(400, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Necromancer>(*textureManager.getTexture("BlackNecromancer"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 6), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Prowler>(*textureManager.getTexture("WhiteProwler"), sf::Vector2f(80, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Familiar>(*textureManager.getTexture("BlackFamiliar"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 2), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<GhostKnight>(*textureManager.getTexture("WhiteGhostKnight"), sf::Vector2f(480, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<BeastKnight>(*textureManager.getTexture("BlackBeastKnight"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 7), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("WhitePawn"), sf::Vector2f(0, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("BlackPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 1), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("WhitePawn"), sf::Vector2f(80, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<HellPawn>(*textureManager.getTexture("BlackHellPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 2), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<HellPawn>(*textureManager.getTexture("WhiteHellPawn"), sf::Vector2f(160, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("BlackPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 3), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("WhitePawn"), sf::Vector2f(240, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<PawnHopper>(*textureManager.getTexture("BlackPawnHopper"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 4), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<NecroPawn>(*textureManager.getTexture("WhiteNecroPawn"), sf::Vector2f(320, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<NecroPawn>(*textureManager.getTexture("BlackNecroPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 5), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("WhitePawn"), sf::Vector2f(400, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("BlackPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 6), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("WhitePawn"), sf::Vector2f(480, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("BlackPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 7), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<YoungWiz>(*textureManager.getTexture("WhiteYoungWiz"), sf::Vector2f(560, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(*textureManager.getTexture("BlackPawn"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 8), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<WizardKing>(*textureManager.getTexture("WhiteWizardKing"), sf::Vector2f(240, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<HellKing>(*textureManager.getTexture("BlackHellKing"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 5), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<QueenOfDestruction>(*textureManager.getTexture("WhiteQueenOfDestruction"), sf::Vector2f(320, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<QueenOfDomination>(*textureManager.getTexture("BlackQueenOfDomination"), sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 4), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));

    float scaleFactor = TILE_SIZE / static_cast<float>(std::max(textureManager.getTexture("WhiteRook")->getSize().x, textureManager.getTexture("WhiteRook")->getSize().y));

    // Scale each piece's sprite to fit the tile size
    for (auto &piece : pieces)
    {
        piece->getSprite().setScale(scaleFactor, scaleFactor);
    }
}

/**
 * @brief Pushes a new pawn piece to the vector of pieces.
 *
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @param pawnTexture The texture to be used for the pawn.
 * @param position The position on the board where the pawn should be placed.
 * @param color The color of the pawn (Piece::Color::White or Piece::Color::Black).
 */
void pushPawn(std::vector<std::unique_ptr<Piece>> &pieces, sf::Texture &pawnTexture, const sf::Vector2f &position, Piece::Color pieceColor)
{
    auto pawn = std::make_unique<Pawn>(pawnTexture, position, pieceColor);
    pawn->getSprite().setScale(static_cast<float>(TILE_SIZE) / pawnTexture.getSize().x, static_cast<float>(TILE_SIZE) / pawnTexture.getSize().y);
    pieces.push_back(std::move(pawn));
}

/**
 * @brief Pushes a new pawn piece to the vector of pieces.
 *
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @param necroPawnTexture The texture to be used for the pawn.
 * @param position The position on the board where the pawn should be placed.
 * @param color The color of the necroPawn (Piece::Color::White or Piece::Color::Black).
 */
void pushNecroPawn(std::vector<std::unique_ptr<Piece>> &pieces, sf::Texture &necroPawnTexture, const sf::Vector2f &position, Piece::Color pieceColor)
{
    auto necroPawn = std::make_unique<NecroPawn>(necroPawnTexture, position, pieceColor);
    necroPawn->getSprite().setScale(static_cast<float>(TILE_SIZE) / necroPawnTexture.getSize().x, static_cast<float>(TILE_SIZE) / necroPawnTexture.getSize().y);

    pieces.push_back(std::move(necroPawn));
}

/**
 * @brief Retrieves the piece located at the specified position on the board.
 *
 * @param position The position on the board to check as an sf::Vector2f.
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @return Piece* A pointer to the Piece at the specified position, or nullptr if no piece is found.
 */
Piece *getPieceAtPosition(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces)
{
    for (const auto &piece : pieces)
    {
        if (piece->getPosition() == position)
        {
            return piece.get();
        }
    }
    return nullptr;
}

/**
 * @brief Checks if the piece at the specified position belongs to the opponent.
 *
 * @param position The position on the board to check as an sf::Vector2f.
 * @param pieces A vector of unique pointers to Piece objects representing all the pieces on the board.
 * @param currentPlayerColor The color of the current player.
 * @return true If the piece at the specified position belongs to the opponent.
 * @return false If there is no piece at the specified position or the piece belongs to the current player.
 */
bool isOpponentPiece(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces, Piece::Color currentPlayerColor)
{
    for (const auto &piece : pieces)
    {
        if (piece->getPosition() == position && piece->getColor() != currentPlayerColor)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Draws the chessboard on the provided window.
 *
 * @param window The SFML RenderWindow where the chessboard will be drawn.
 * @param board A 2D vector of Square objects representing the chessboard.
 */
void drawChessboard(sf::RenderWindow &window, std::vector<std::vector<Square>> &board)
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            board[row][col].draw(window);
        }
    }
}

/**
 * @brief Captures user input from the keyboard to determine movement direction.
 *
 * @return sf::Vector2i A vector representing the direction of movement:
 *                      (-1, 0) for left, (1, 0) for right, (0, -1) for up, (0, 1) for down.
 */
sf::Vector2i getUserInput()
{
    sf::Vector2i movement(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        movement.y = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        movement.y = 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        movement.x = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        movement.x = 1;
    }
    return movement;
}

/**
 * @brief Calculates the next position of a piece based on the current position and movement direction.
 *
 * @param currentPosition The current position of the piece as an sf::Vector2f.
 * @param movement The direction of movement as an sf::Vector2i.
 * @return sf::Vector2f The next position of the piece, constrained within the bounds of the chessboard.
 */
sf::Vector2f calculateNextPosition(const sf::Vector2f &currentPosition, const sf::Vector2i &movement)
{
    // Calculate the next position based on the movement
    sf::Vector2f nextPosition = currentPosition + sf::Vector2f(movement.x * TILE_SIZE, movement.y * TILE_SIZE);

    // Ensure the next position stays within the bounds of the chessboard
    nextPosition.x = std::max(0.0f, std::min(nextPosition.x, static_cast<float>(BOARD_SIZE - 1) * TILE_SIZE));
    nextPosition.y = std::max(0.0f, std::min(nextPosition.y, static_cast<float>(BOARD_SIZE - 1) * TILE_SIZE));

    return nextPosition;
}

/*
 * @brief Changes the button color when the mouse hovers over it.
 *
 * @param window The SFML RenderWindow to check for mouse position.
 * @param button The Button object to be highlighted.
 * @param normalColor The color of the button when not hovered.
 * @param hoverColor The color of the button when hovered.
 */
void highlightButtonOnHover(sf::RenderWindow &window, Button &button, const sf::Color &normalColor, const sf::Color &hoverColor)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (button.getButtonShape().getGlobalBounds().contains(mousePosF))
    {
        button.getButtonShape().setFillColor(hoverColor);
    }
    else
    {
        button.getButtonShape().setFillColor(normalColor);
    }
}