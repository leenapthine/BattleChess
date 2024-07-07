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
#include "king.h"
#include "queen.h"
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
#include "howler.h"
#include "familiar.h"
#include "prowler.h"
#include "hellPawn.h"
#include "youngWiz.h"

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
void loadTextures(std::vector<std::unique_ptr<Piece>> &pieces)
{
    if (!whiteRookTexture.loadFromFile("../resources/WhiteRook.png"))
    {
        throw std::runtime_error("Failed to load WhiteRook.png");
    }
    if (!blackRookTexture.loadFromFile("../resources/BlackRook.png"))
    {
        throw std::runtime_error("Failed to load BlackRook.png");
    }

    if (!whiteBishopTexture.loadFromFile("../resources/WhiteBishop.png"))
    {
        throw std::runtime_error("Failed to load WhiteBishop.png");
    }
    if (!blackBishopTexture.loadFromFile("../resources/BlackBishop.png"))
    {
        throw std::runtime_error("Failed to load BlackBishop.png");
    }

    if (!whitePawnTexture.loadFromFile("../resources/WhitePawn.png"))
    {
        throw std::runtime_error("Failed to load WhitePawn.png");
    }
    if (!blackPawnTexture.loadFromFile("../resources/BlackPawn.png"))
    {
        throw std::runtime_error("Failed to load BlackPawn.png");
    }

    if (!whiteKnightTexture.loadFromFile("../resources/WhiteKnight.png"))
    {
        throw std::runtime_error("Failed to load WhiteKnight.png");
    }
    if (!blackKnightTexture.loadFromFile("../resources/BlackKnight.png"))
    {
        throw std::runtime_error("Failed to load BlackKnight.png");
    }

    if (!whiteKingTexture.loadFromFile("../resources/WhiteKing.png"))
    {
        throw std::runtime_error("Failed to load WhiteKing.png");
    }
    if (!blackKingTexture.loadFromFile("../resources/BlackKing.png"))
    {
        throw std::runtime_error("Failed to load BlackKing.png");
    }

    if (!whiteQueenTexture.loadFromFile("../resources/WhiteQueen.png"))
    {
        throw std::runtime_error("Failed to load WhiteQueen.png");
    }
    if (!blackQueenTexture.loadFromFile("../resources/BlackQueen.png"))
    {
        throw std::runtime_error("Failed to load BlackQueen.png");
    }

    if (!whiteWizardTexture.loadFromFile("../resources/WhiteWizard.png"))
    {
        throw std::runtime_error("Failed to load WhiteWizard.png");
    }
    if (!blackWizardTexture.loadFromFile("../resources/BlackWizard.png"))
    {
        throw std::runtime_error("Failed to load BlackWizard.png");
    }

    if (!whiteNecromancerTexture.loadFromFile("../resources/WhiteNecromancer.png"))
    {
        throw std::runtime_error("Failed to load WhiteNecromancer.png");
    }
    if (!blackNecromancerTexture.loadFromFile("../resources/BlackNecromancer.png"))
    {
        throw std::runtime_error("Failed to load BlackNecromancer.png");
    }

    if (!whiteNecroPawnTexture.loadFromFile("../resources/WhiteNecroPawn.png"))
    {
        throw std::runtime_error("Failed to load WhiteNecroPawn.png");
    }
    if (!blackNecroPawnTexture.loadFromFile("../resources/BlackNecroPawn.png"))
    {
        throw std::runtime_error("Failed to load BlackNecroPawn.png");
    }

    if (!whiteBeastKnightTexture.loadFromFile("../resources/WhiteBeastKnight.png"))
    {
        throw std::runtime_error("Failed to load WhiteBeastKnight.png");
    }
    if (!blackBeastKnightTexture.loadFromFile("../resources/BlackBeastKnight.png"))
    {
        throw std::runtime_error("Failed to load BlackBeastKnight.png");
    }

    if (!whiteBoulderThrowerTexture.loadFromFile("../resources/WhiteBoulderThrower.png"))
    {
        throw std::runtime_error("Failed to load WhiteBoulderThrower.png");
    }
    if (!blackBoulderThrowerTexture.loadFromFile("../resources/BlackBoulderThrower.png"))
    {
        throw std::runtime_error("Failed to load BlackBoulderThrower.png");
    }

    if (!whiteHowlerTexture.loadFromFile("../resources/WhiteHowler.png"))
    {
        throw std::runtime_error("Failed to load WhiteHowler.png");
    }
    if (!blackHowlerTexture.loadFromFile("../resources/BlackHowler.png"))
    {
        throw std::runtime_error("Failed to load BlackHowler.png");
    }

    if (!whiteProwlerTexture.loadFromFile("../resources/WhiteProwler.png"))
    {
        throw std::runtime_error("Failed to load WhiteProwler.png");
    }
    if (!blackProwlerTexture.loadFromFile("../resources/BlackProwler.png"))
    {
        throw std::runtime_error("Failed to load BlackProwler.png");
    }

    if (!whiteHellPawnTexture.loadFromFile("../resources/WhiteHellPawn.png"))
    {
        throw std::runtime_error("Failed to load WhiteHellPawn.png");
    }
    if (!blackHellPawnTexture.loadFromFile("../resources/BlackHellPawn.png"))
    {
        throw std::runtime_error("Failed to load BlackHellPawn.png");
    }

    if (!whiteYoungWizTexture.loadFromFile("../resources/WhiteYoungWiz.png"))
    {
        throw std::runtime_error("Failed to load WhiteYoungWiz.png");
    }
    if (!blackYoungWizTexture.loadFromFile("../resources/BlackYoungWiz.png"))
    {
        throw std::runtime_error("Failed to load BlackYoungWiz.png");
    }

    if (!whitePawnHopperTexture.loadFromFile("../resources/WhitePawnHopper.png"))
    {
        throw std::runtime_error("Failed to load WhitePawnHopper.png");
    }
    if (!blackPawnHopperTexture.loadFromFile("../resources/BlackPawnHopper.png"))
    {
        throw std::runtime_error("Failed to load BlackPawnHopper.png");
    }

    if (!whiteBeastDruidTexture.loadFromFile("../resources/WhiteBeastDruid.png"))
    {
        throw std::runtime_error("Failed to load WhiteBeastDruid.png");
    }
    if (!blackBeastDruidTexture.loadFromFile("../resources/BlackBeastDruid.png"))
    {
        throw std::runtime_error("Failed to load BlackBeastDruid.png");
    }

    if (!whiteGhostKnightTexture.loadFromFile("../resources/WhiteGhostKnight.png"))
    {
        throw std::runtime_error("Failed to load WhiteGhostKnight.png");
    }
    if (!blackGhostKnightTexture.loadFromFile("../resources/BlackGhostKnight.png"))
    {
        throw std::runtime_error("Failed to load BlackGhostKnight.png");
    }

    if (!whiteFamiliarTexture.loadFromFile("../resources/WhiteFamiliar.png"))
    {
        throw std::runtime_error("Failed to load WhiteFamiliar.png");
    }
    if (!blackFamiliarTexture.loadFromFile("../resources/BlackFamiliar.png"))
    {
        throw std::runtime_error("Failed to load BlackFamiliar.png");
    }

    // Clear the pieces vector before adding new pieces
    pieces.clear();

    // Add new pieces to the pieces vector
    pieces.push_back(std::make_unique<Rook>(whiteRookTexture, sf::Vector2f(0, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Rook>(blackRookTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 1), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Rook>(whiteRookTexture, sf::Vector2f(560, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<BoulderThrower>(blackBoulderThrowerTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 8), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Howler>(whiteHowlerTexture, sf::Vector2f(160, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<BeastDruid>(blackBeastDruidTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 3), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Wizard>(whiteWizardTexture, sf::Vector2f(400, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Necromancer>(blackNecromancerTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 6), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Prowler>(whiteProwlerTexture, sf::Vector2f(80, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Familiar>(blackFamiliarTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 2), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<GhostKnight>(whiteGhostKnightTexture, sf::Vector2f(480, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<BeastKnight>(blackBeastKnightTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 7), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(whitePawnTexture, sf::Vector2f(0, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(blackPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 1), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(whitePawnTexture, sf::Vector2f(80, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<HellPawn>(blackHellPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 2), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<HellPawn>(whiteHellPawnTexture, sf::Vector2f(160, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(blackPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 3), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(whitePawnTexture, sf::Vector2f(240, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<PawnHopper>(blackPawnHopperTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 4), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<NecroPawn>(whiteNecroPawnTexture, sf::Vector2f(320, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<NecroPawn>(blackNecroPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 5), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(whitePawnTexture, sf::Vector2f(400, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(blackPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 6), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Pawn>(whitePawnTexture, sf::Vector2f(480, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(blackPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 7), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<YoungWiz>(whiteYoungWizTexture, sf::Vector2f(560, 80), Piece::Color::White));
    pieces.push_back(std::make_unique<Pawn>(blackPawnTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 8), TILE_SIZE * (BOARD_SIZE - 2)), Piece::Color::Black));
    pieces.push_back(std::make_unique<King>(whiteKingTexture, sf::Vector2f(320, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<King>(blackKingTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 5), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    pieces.push_back(std::make_unique<Queen>(whiteQueenTexture, sf::Vector2f(240, 0), Piece::Color::White));
    pieces.push_back(std::make_unique<Queen>(blackQueenTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 4), TILE_SIZE * (BOARD_SIZE - 1)), Piece::Color::Black));
    // pieces.push_back(std::make_unique<Wizard>(whiteWizardTexture, sf::Vector2f(240, 160), Piece::Color::White));
    // pieces.push_back(std::make_unique<Wizard>(blackWizardTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 4), TILE_SIZE * (BOARD_SIZE - 3)), Piece::Color::Black));
    // pieces.push_back(std::make_unique<Necromancer>(whiteNecromancerTexture, sf::Vector2f(320, 160), Piece::Color::White));
    // pieces.push_back(std::make_unique<Necromancer>(blackNecromancerTexture, sf::Vector2f(TILE_SIZE * (BOARD_SIZE - 3), TILE_SIZE * (BOARD_SIZE - 3)), Piece::Color::Black));

    float scaleFactor = TILE_SIZE / static_cast<float>(std::max(whiteRookTexture.getSize().x, whiteRookTexture.getSize().y));

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