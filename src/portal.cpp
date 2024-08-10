// Filename: portal.cpp
// Description: Implementation of the Portal class, which defines the behavior and movement rules of the Portal piece in our chess game.

// Main Classes:
// - Portal: Represents the Portal piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool Portal::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the Portal can legally move to the specified target position on the chessboard.
// - void Portal::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the Portal on the provided chessboard, considering its current position and game rules.
// - void Portal::captureWithPortals(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<Portal *> &portals, std::vector<std::vector<Square>> &board):
//   Loads a friendly piece into a portal, and adds the piece pointer to all other portals.
// - void Portal::teleport(sf::RenderWindow &window, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, bool isWhiteTurn, std::vector<Portal *> &portals)
//   Unloads a piece from a portal, and sets the piece pointer to null for all other portals
// - void Portal::highlightValidUnloadPos(const sf::Vector2f &position, std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces)
//   Highlight all valid places where a portal can unload a loaded piece.
// - void Portal::highlightAdjacentPieces(std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board, bool isWhiteTurn)
//   Highlights all pieces in the perimeter of a portal that can load into a portal.

// Special Features or Notes:
// - The Portal is a level 2 Rook piece that belongs to the Wizard class.
// - The Portal moves in straight lines in any direction, so long as the path is unobstructed.
// - The Portal captures on movement into a space occupied by an opponent piece.
// - The Portal has a special ability where any piece in a portals perimeter can enter a portal
//   and exit out a portal prior to a portals movement for turn.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the Portal piece.

#include "portal.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>
#include <iostream>

// Determine if the Portal can move to a specified target position
bool Portal::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Portal can only move in straight lines
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
    return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
}

// Highlight valid moves for the Portal on the provided board
void Portal::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();

    // Convert the floating-point coordinates to integer indices
    int row = static_cast<int>(currentPosition.y / TILE_SIZE);
    int col = static_cast<int>(currentPosition.x / TILE_SIZE);

    // Ensure the indices are within the valid range of the board dimensions
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        // Highlight the current position in red
        board[row][col].setHighlight(true, sf::Color::Green);
    }

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Vector2f horizontalTarget(i * TILE_SIZE, currentPosition.y);

        // Iterates through all valid movement spaces, and highlights according to a movement (yellow) or a capture (red)
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

        // Iterates through all valid movement spaces, and highlights according to a movement (yellow) or a capture (red)
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
    }
}

void Portal::captureWithPortals(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<Portal *> &portals, std::vector<std::vector<Square>> &board)
{
    auto it = std::find_if(pieces.begin(), pieces.end(),
                           [&target](const std::unique_ptr<Piece> &piece)
                           {
                               return piece->getPosition() == target;
                           });

    if (it != pieces.end() && (*it)->getColor() == getColor())
    {
        Piece *capturedPiece = it->release(); // Store the captured piece
        pieces.erase(it);                     // Remove the piece from the pieces vector

        // Update all portals of the same color
        for (auto &portal : portals)
        {
            if (portal->getColor() == getColor())
            {
                portal->pieceInPortal = capturedPiece;
                portal->pieceLoaded = true;

                // Print details for debugging
                std::cout << "Portal at position (" << portal->getPosition().x / TILE_SIZE << ", "
                          << portal->getPosition().y / TILE_SIZE << ") has piece: ";
                portal->pieceInPortal->print(std::cout);
                std::cout << std::endl;

                // Clear the highlights for the adjacent squares
                int col = portal->getPosition().x / TILE_SIZE;
                int row = portal->getPosition().y / TILE_SIZE;

                for (const auto &dir : portal->directions)
                {
                    int newRow = row + dir[0];
                    int newCol = col + dir[1];

                    if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
                    {
                        board[newRow][newCol].setHighlight(false);
                    }
                }
            }
        }
    }
}

void Portal::teleport(sf::RenderWindow &window, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, bool isWhiteTurn, std::vector<Portal *> &portals)
{
    sf::Vector2f currentPosition = getPosition();

    if (pieceLoaded)
    {
        // Set unload highlights
        highlightValidUnloadPos(currentPosition, board, pieces);

        // Redraw the board
        window.clear();
        drawChessboard(window, board);
        for (const auto &piece : pieces)
        {
            window.draw(piece->getSprite());
        }
        window.display();

        bool validUnloadPositionSelected = false;

        while (!validUnloadPositionSelected)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    int clickX = event.mouseButton.x;
                    int clickY = event.mouseButton.y;
                    int clickRow = clickY / TILE_SIZE;
                    int clickCol = clickX / TILE_SIZE;

                    sf::Vector2f clickPosition(clickCol * TILE_SIZE, clickRow * TILE_SIZE);

                    // Check if the clicked position is the portal itself
                    if (clickPosition == currentPosition || !board[clickRow][clickCol].getHighlight())
                    {
                        std::cout << "Clicked on the portal itself or a non-highlighted position." << std::endl;

                        // Clear all highlights
                        for (int r = 0; r < BOARD_SIZE; ++r)
                        {
                            for (int c = 0; c < BOARD_SIZE; ++c)
                            {
                                board[r][c].setHighlight(false);
                            }
                        }

                        // Redraw the board without highlights
                        window.clear();
                        drawChessboard(window, board);
                        for (const auto &piece : pieces)
                        {
                            window.draw(piece->getSprite());
                        }
                        window.display();

                        validUnloadPositionSelected = true;
                        break;
                    }

                    // Check if the clicked position is highlighted
                    if (board[clickRow][clickCol].getHighlight())
                    {
                        // Unload the piece from the portal
                        pieceInPortal->setPosition(clickPosition);
                        pieces.push_back(std::unique_ptr<Piece>(pieceInPortal));
                        pieceInPortal = nullptr;
                        pieceLoaded = false;
                        validUnloadPositionSelected = true;

                        std::cout << "Piece is unloaded from the portal!" << std::endl;

                        // Clear all highlights after unloading the piece
                        for (int r = 0; r < BOARD_SIZE; ++r)
                        {
                            for (int c = 0; c < BOARD_SIZE; ++c)
                            {
                                board[r][c].setHighlight(false);
                            }
                        }

                        // Update all portals of the same color
                        for (auto &portal : portals)
                        {
                            if (portal->getColor() == getColor())
                            {
                                portal->pieceInPortal = nullptr;
                                portal->pieceLoaded = false;
                            }
                        }

                        // Redraw the board without highlights
                        window.clear();
                        drawChessboard(window, board);
                        for (const auto &piece : pieces)
                        {
                            window.draw(piece->getSprite());
                        }
                        window.display();
                        break;
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "No piece loaded in the portal." << std::endl;
    }
}

void Portal::highlightValidUnloadPos(const sf::Vector2f &position, std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces)
{
    sf::Vector2f portalPosition = getPosition();

    // Clear all highlights before setting new ones
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            board[r][c].setHighlight(false);
        }
    }

    // Check the portal's position in terms of row and column
    int col = portalPosition.x / TILE_SIZE;
    int row = portalPosition.y / TILE_SIZE;

    for (const auto &dir : directions)
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        // Check if the new position is within the board bounds
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
        {
            // Check if the position is not occupied by another piece
            bool isOccupied = false;
            for (const auto &piece : pieces)
            {
                if (piece->getPosition() == sf::Vector2f(newCol * TILE_SIZE, newRow * TILE_SIZE))
                {
                    isOccupied = true;
                    break;
                }
            }

            if (!isOccupied)
            {
                // Highlight the square
                board[newRow][newCol].setHighlight(true, sf::Color::Green);
            }
        }
    }
}

void Portal::highlightAdjacentPieces(std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board, bool isWhiteTurn)
{
    int col = getPosition().x / TILE_SIZE;
    int row = getPosition().y / TILE_SIZE;

    for (const auto &dir : directions)
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
        {
            Piece *adjacentPiece = getPieceAtPosition(sf::Vector2f(newCol * TILE_SIZE, newRow * TILE_SIZE), pieces);
            if (adjacentPiece && (adjacentPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black)))
            {
                board[newRow][newCol].setHighlight(true, sf::Color::Red);
            }
        }
    }
}