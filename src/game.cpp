#include "game.h"
#include "wizard.h"
#include "youngWiz.h"
#include "necromancer.h"
#include "pawn.h"
#include "necroPawn.h"
#include "hellPawn.h"
#include "boulderThrower.h"
#include "howler.h"
#include "utility.h"
#include "globals.h"
#include "piece.h"
#include <iostream>

/**
 * @brief Runs the main chess game loop.
 *
 * This function sets up the game board, loads the piece textures, handles user interactions,
 * updates the game state, and renders the game board and pieces.
 * It also opperates capture positions and some specific logic for the unique game pieces.
 *
 * @param window Reference to the SFML window where the game is rendered.
 */

void runChessGame(sf::RenderWindow &window)
{
    std::vector<std::unique_ptr<Piece>> pieces;
    try
    {
        loadTextures(pieces); // Load textures for all pieces
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error loading textures: " << e.what() << std::endl;
        return;
    }

    std::vector<std::vector<Square>> board(BOARD_SIZE, std::vector<Square>(BOARD_SIZE, Square(sf::Vector2f(), sf::Color::White)));

    sf::Color darkGreen(0, 100, 0);
    sf::Color lightGreen(144, 238, 144);

    // Initialize the chessboard squares
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            sf::Vector2f position(col * TILE_SIZE, row * TILE_SIZE);
            sf::Color color = (row + col) % 2 == 0 ? lightGreen : darkGreen;
            board[row][col] = Square(position, color);
        }
    }

    bool pieceSelected = false;
    Piece *selectedPiece = nullptr;

    // Necromancer specific variables
    bool awaitingPawnPlacement = false;
    Necromancer *necromancer = nullptr;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / TILE_SIZE;
                int row = mousePos.y / TILE_SIZE;
                sf::Vector2f selectedPiecePosition = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);

                if (awaitingPawnPlacement)
                {
                    if (board[row][col].getHighlight())
                    {
                        pushPawn(pieces, necromancer->getPawnTexture(), selectedPiecePosition, necromancer->getColor());
                        awaitingPawnPlacement = false;
                        necromancer = nullptr;

                        // Only clear highlights related to the pawn placement, not all highlights
                        for (int r = 0; r < BOARD_SIZE; ++r)
                        {
                            for (int c = 0; c < BOARD_SIZE; ++c)
                            {
                                if (board[r][c].getHighlight())
                                {
                                    board[r][c].setHighlight(false);
                                }
                            }
                        }
                    }
                    continue;
                }

                Piece *clickedPiece = getPieceAtPosition(selectedPiecePosition, pieces);

                if (clickedPiece)
                {
                    if (!pieceSelected)
                    {
                        pieceSelected = true;
                        selectedPiece = clickedPiece;
                        selectedPiece->highlightValidMoves(board, pieces);

                        // Highlight capture zones for BoulderThrower
                        if (selectedPiece->getType() == "BoulderThrower")
                        {
                            auto &boulderThrower = static_cast<BoulderThrower &>(*selectedPiece);
                            boulderThrower.highlightCaptureZones(board, pieces);
                        }
                    }
                    else if (selectedPiece == clickedPiece)
                    {
                        if (selectedPiece->getType() == "NecroPawn")
                        {
                            auto &necroPawn = static_cast<NecroPawn &>(*selectedPiece);
                            necroPawn.sacrifice(pieces);
                            pieceSelected = false;
                            selectedPiece = nullptr;

                            // Clear all highlights after sacrifice
                            for (int r = 0; r < BOARD_SIZE; ++r)
                            {
                                for (int c = 0; c < BOARD_SIZE; ++c)
                                {
                                    board[r][c].setHighlight(false);
                                }
                            }
                        }
                        else
                        {
                            pieceSelected = false;
                            selectedPiece = nullptr;

                            // Clear all highlights
                            for (int r = 0; r < BOARD_SIZE; ++r)
                            {
                                for (int c = 0; c < BOARD_SIZE; ++c)
                                {
                                    board[r][c].setHighlight(false);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (clickedPiece->getColor() == selectedPiece->getColor())
                        {
                            pieceSelected = true;
                            selectedPiece = clickedPiece;
                            for (int r = 0; r < BOARD_SIZE; ++r)
                            {
                                for (int c = 0; c < BOARD_SIZE; ++c)
                                {
                                    board[r][c].setHighlight(false);
                                }
                            }
                            selectedPiece->highlightValidMoves(board, pieces);
                        }
                        else
                        {
                            sf::Vector2f targetPosition = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);

                            if (board[row][col].getHighlight())
                            {
                                Piece *targetPiece = getPieceAtPosition(targetPosition, pieces);

                                if (targetPiece && targetPiece != selectedPiece)
                                {
                                    selectedPiece->capture(targetPosition, pieces);

                                    if (selectedPiece->getType() == "Necromancer")
                                    {
                                        // Clear highlights before entering raiseDead
                                        for (int r = 0; r < BOARD_SIZE; ++r)
                                        {
                                            for (int c = 0; c < BOARD_SIZE; ++c)
                                            {
                                                board[r][c].setHighlight(false);
                                            }
                                        }
                                        // Necromancer raises dead
                                        necromancer = static_cast<Necromancer *>(selectedPiece);
                                        awaitingPawnPlacement = necromancer->raiseDead(targetPosition, board, pieces);
                                    }
                                }
                                else
                                {
                                    selectedPiece->setPosition(targetPosition);
                                }

                                pieceSelected = false;
                                selectedPiece = nullptr;

                                // Clear all highlights except those related to awaiting pawn placement
                                if (!awaitingPawnPlacement)
                                {
                                    for (int r = 0; r < BOARD_SIZE; ++r)
                                    {
                                        for (int c = 0; c < BOARD_SIZE; ++c)
                                        {
                                            board[r][c].setHighlight(false);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (pieceSelected)
                {
                    sf::Vector2f targetPosition = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);

                    if (board[row][col].getHighlight())
                    {
                        Piece *targetPiece = getPieceAtPosition(targetPosition, pieces);
                        if (targetPiece && targetPiece != selectedPiece)
                        {
                            selectedPiece->capture(targetPosition, pieces);

                            if (selectedPiece->getType() == "Necromancer")
                            {
                                // Clear highlights before entering raiseDead
                                for (int r = 0; r < BOARD_SIZE; ++r)
                                {
                                    for (int c = 0; c < BOARD_SIZE; ++c)
                                    {
                                        board[r][c].setHighlight(false);
                                    }
                                }
                                // Necromancer raises dead
                                necromancer = static_cast<Necromancer *>(selectedPiece);
                                awaitingPawnPlacement = necromancer->raiseDead(targetPosition, board, pieces);
                            }
                        }
                        else
                        {
                            selectedPiece->setPosition(targetPosition);
                        }

                        pieceSelected = false;
                        selectedPiece = nullptr;

                        // Clear all highlights except those related to awaiting pawn placement
                        if (!awaitingPawnPlacement)
                        {
                            for (int r = 0; r < BOARD_SIZE; ++r)
                            {
                                for (int c = 0; c < BOARD_SIZE; ++c)
                                {
                                    board[r][c].setHighlight(false);
                                }
                            }
                        }
                    }
                }
            }
        }
        // code to remove pieces marked for removal
        pieces.erase(std::remove_if(pieces.begin(), pieces.end(),
                                    [](const std::unique_ptr<Piece> &piece)
                                    {
                                        auto hellPawn = dynamic_cast<HellPawn *>(piece.get());
                                        return hellPawn && hellPawn->toBeRemoved;
                                    }),
                     pieces.end());

        window.clear();
        drawChessboard(window, board);
        for (const auto &piece : pieces)
        {
            window.draw(piece->getSprite());
        }
        window.display();
    }
}