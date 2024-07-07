#include "game.h"
#include "piece.h"
#include "wizard.h"
#include "youngWiz.h"
#include "necromancer.h"
#include "pawn.h"
#include "pawnHopper.h"
#include "necroPawn.h"
#include "hellPawn.h"
#include "boulderThrower.h"
#include "howler.h"
#include "prowler.h"
#include "familiar.h"
#include "ghostKnight.h"
#include "utility.h"
#include "globals.h"
#include "piece.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>

/**
 * @brief Runs the main chess game loop.
 *
 * This function sets up the game board, loads the piece textures, handles user interactions,
 * updates the game state, and renders the game board and pieces.
 * It also operates capture positions and some specific logic for the unique game pieces.
 *
 * @param window Reference to the SFML window where the game is rendered.
 */
void Game::runChessGame(sf::RenderWindow &window)
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

    // Initialize ghostKnight variable
    GhostKnight *ghostKnight = nullptr;

    // Prowler specific variables
    bool prowlerNeedsAdditionalMove = false;
    Piece *prowlerForAdditionalMove = nullptr;

    // Load font for turn indicator
    sf::Font font;
    if (!font.loadFromFile("../resources/JmhcthulhumbusarcadeugRegular-JleB.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    // Turn indicator text
    sf::Text turnIndicator;
    turnIndicator.setFont(font);
    turnIndicator.setCharacterSize(48);
    turnIndicator.setFillColor(sf::Color::White);
    turnIndicator.setStyle(sf::Text::Bold);
    turnIndicator.setOutlineColor(sf::Color::Black);
    turnIndicator.setOutlineThickness(2);

    // Timer and fade effect variables
    sf::Clock turnIndicatorClock;
    sf::Time turnIndicatorDisplayTime = sf::seconds(1); // Display for 1 second
    bool showTurnIndicator = true;
    sf::Clock fadeClock;

    bool isWhiteTurn = true; // White starts the game

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

                if (prowlerNeedsAdditionalMove)
                {
                    if (clickedPiece == prowlerForAdditionalMove)
                    {
                        pieceSelected = true;
                        selectedPiece = clickedPiece;
                        selectedPiece->highlightValidMoves(board, pieces);

                        // Proceed to the actual move logic if the prowler is clicked again for an additional move
                        continue;
                    }
                    else if (selectedPiece && board[row][col].getHighlight())
                    {
                        sf::Vector2f targetPosition = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);
                        Piece *targetPiece = getPieceAtPosition(targetPosition, pieces);

                        if (targetPiece && targetPiece != selectedPiece)
                        {
                            selectedPiece->capture(targetPosition, pieces);
                        }
                        else
                        {
                            selectedPiece->setPosition(targetPosition);
                        }

                        prowlerNeedsAdditionalMove = false;
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

                        isWhiteTurn = !isWhiteTurn; // Switch turn after Prowler's additional move
                        // pieceSelected = true;
                        // selectedPiece = clickedPiece;

                        // Reset the stunned state of all pieces of the current player
                        for (auto &piece : pieces)
                        {
                            if ((isWhiteTurn && piece->getColor() == Piece::Color::Black) ||
                                (!isWhiteTurn && piece->getColor() == Piece::Color::White))
                            {
                                piece->setStunned(false);
                            }
                        }

                        turnIndicator.setString(isWhiteTurn ? "White's Turn" : "Black's Turn");
                        turnIndicator.setFillColor(sf::Color::White);
                        showTurnIndicator = true;
                        turnIndicatorClock.restart();
                        fadeClock.restart();

                        // Center the turn indicator
                        sf::FloatRect textRect = turnIndicator.getLocalBounds();
                        turnIndicator.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                        turnIndicator.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
                    }
                    else
                    {
                        // Ignore clicks on other pieces during the Prowler's additional move
                        continue;
                    }
                }

                else if (clickedPiece)
                {
                    if (!pieceSelected)
                    {

                        pieceSelected = true;
                        selectedPiece = clickedPiece;

                        if (selectedPiece->isStunned())
                        {
                            // Provide feedback that the piece is stunned
                            std::cerr << "Selected piece is stunned and cannot move this turn." << std::endl;
                            pieceSelected = false;
                            selectedPiece = nullptr;
                            continue;
                        }
                        else
                        {
                            // Familiar wakes up
                            if (selectedPiece->getType() == "Familiar")
                            {
                                auto &familiar = static_cast<Familiar &>(*selectedPiece);
                                familiar.setStone(false);
                            }

                            selectedPiece->highlightValidMoves(board, pieces);
                        }

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
                            // Check if it's the turn of the selected piece's color
                            if ((selectedPiece->getColor() == Piece::Color::White && isWhiteTurn) ||
                                (selectedPiece->getColor() == Piece::Color::Black && !isWhiteTurn))
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

                                // Switch turn after the sacrifice
                                isWhiteTurn = !isWhiteTurn;

                                // Reset the stunned state of all pieces of the current player
                                for (auto &piece : pieces)
                                {
                                    if ((isWhiteTurn && piece->getColor() == Piece::Color::Black) ||
                                        (!isWhiteTurn && piece->getColor() == Piece::Color::White))
                                    {
                                        piece->setStunned(false);
                                    }
                                }
                                turnIndicator.setString(isWhiteTurn ? "White's Turn" : "Black's Turn");
                                turnIndicator.setFillColor(sf::Color::White);
                                showTurnIndicator = true;
                                turnIndicatorClock.restart();
                                fadeClock.restart();

                                // Center the turn indicator
                                sf::FloatRect textRect = turnIndicator.getLocalBounds();
                                turnIndicator.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                                turnIndicator.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
                            }
                            else
                            {
                                // Clear all highlights
                                for (int r = 0; r < BOARD_SIZE; ++r)
                                {
                                    for (int c = 0; c < BOARD_SIZE; ++c)
                                    {
                                        board[r][c].setHighlight(false);
                                    }
                                }
                                // Reset selected state
                                pieceSelected = false;
                                selectedPiece = nullptr;
                            }
                        }
                        // Familiar turns to stone
                        else if (selectedPiece->getType() == "Familiar" &&
                                     (isWhiteTurn && selectedPiece->getColor() == Piece::Color::White) ||
                                 (!isWhiteTurn && selectedPiece->getColor() == Piece::Color::Black))
                        {
                            auto &familiar = static_cast<Familiar &>(*selectedPiece);
                            familiar.setStone(true);

                            // Clear all highlights after turned to stone
                            for (int r = 0; r < BOARD_SIZE; ++r)
                            {
                                for (int c = 0; c < BOARD_SIZE; ++c)
                                {
                                    board[r][c].setHighlight(false);
                                }
                            }

                            std::cout << "Familiar isStone: (" << familiar.isStone() << ")\n";
                            // Switch turn after the sacrifice
                            isWhiteTurn = !isWhiteTurn;
                            pieceSelected = false;
                            selectedPiece = nullptr;

                            // Reset the stunned state of all pieces of the current player
                            for (auto &piece : pieces)
                            {
                                if ((isWhiteTurn && piece->getColor() == Piece::Color::Black) ||
                                    (!isWhiteTurn && piece->getColor() == Piece::Color::White))
                                {
                                    piece->setStunned(false);
                                }
                            }
                            turnIndicator.setString(isWhiteTurn ? "White's Turn" : "Black's Turn");
                            turnIndicator.setFillColor(sf::Color::White);
                            showTurnIndicator = true;
                            turnIndicatorClock.restart();
                            fadeClock.restart();

                            // Center the turn indicator
                            sf::FloatRect textRect = turnIndicator.getLocalBounds();
                            turnIndicator.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                            turnIndicator.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
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

                            if (board[row][col].getHighlight() && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black))
                            {
                                Piece *targetPiece = getPieceAtPosition(targetPosition, pieces);
                                bool canCapture = true;

                                if (targetPiece && targetPiece != selectedPiece)
                                {
                                    if (targetPiece->getType() == "Familiar")
                                    {
                                        Familiar *familiar = static_cast<Familiar *>(targetPiece);
                                        if (familiar->isStone())
                                        {
                                            std::cout << "Cannot capture Familiar: it is turned to stone. Hi" << std::endl;
                                            canCapture = false;
                                        }
                                    }

                                    if (canCapture)
                                    {
                                        selectedPiece->capture(targetPosition, pieces);
                                    }

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
                                    else if (selectedPiece->getType() == "Prowler")
                                    {
                                        prowlerNeedsAdditionalMove = true;
                                        prowlerForAdditionalMove = selectedPiece;
                                    }
                                    else if (selectedPiece->getType() == "GhostKnight")
                                    {
                                        if (targetPiece->getType() == "Familiar")
                                        {
                                            if (canCapture)
                                            {
                                                ghostKnight = static_cast<GhostKnight *>(selectedPiece);
                                                ghostKnight->stunAdjacentEnemies(targetPosition, pieces, board);
                                            }
                                        }
                                    }
                                }
                                /*
                                else
                                {
                                    // Move the selected piece to the target position
                                    selectedPiece->setPosition(targetPosition);
                                    if (selectedPiece->getType() == "GhostKnight")
                                    {
                                        // sf::Vector2f stunPosition = selectedPiece->getPosition();
                                        ghostKnight = static_cast<GhostKnight *>(selectedPiece);
                                        ghostKnight->stunAdjacentEnemies(targetPosition, pieces, board);
                                    }
                                }
                                */
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
                                if (canCapture)
                                {
                                    if (!prowlerNeedsAdditionalMove)
                                    {
                                        // Switch turn
                                        isWhiteTurn = !isWhiteTurn;
                                    }

                                    // Reset the stunned state of all pieces of the current player
                                    for (auto &piece : pieces)
                                    {
                                        if ((isWhiteTurn && piece->getColor() == Piece::Color::Black) ||
                                            (!isWhiteTurn && piece->getColor() == Piece::Color::White))
                                        {
                                            piece->setStunned(false);
                                        }
                                    }
                                    turnIndicator.setString(prowlerNeedsAdditionalMove ? "Prowler can move again" : (isWhiteTurn ? "White's Turn" : "Black's Turn"));
                                    turnIndicator.setFillColor(sf::Color::White);
                                    showTurnIndicator = true;
                                    turnIndicatorClock.restart();
                                    fadeClock.restart();

                                    // Center the turn indicator
                                    sf::FloatRect textRect = turnIndicator.getLocalBounds();
                                    turnIndicator.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                                    turnIndicator.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
                                }
                            }
                        }
                    }
                }
                else if (pieceSelected)
                {
                    sf::Vector2f targetPosition = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);

                    if (board[row][col].getHighlight() && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black))
                    {
                        Piece *targetPiece = getPieceAtPosition(targetPosition, pieces);
                        bool canCapture = true;

                        if (targetPiece && targetPiece != selectedPiece)
                        {
                            if (targetPiece->getType() == "Familiar")
                            {
                                Familiar *familiar = static_cast<Familiar *>(targetPiece);
                                if (familiar->isStone())
                                {
                                    std::cout << "Cannot capture Familiar: it is turned to stone. Hi" << std::endl;
                                    canCapture = false;
                                }
                            }

                            if (canCapture)
                            {
                                selectedPiece->capture(targetPosition, pieces);
                            }

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
                            else if (selectedPiece->getType() == "Prowler")
                            {
                                prowlerNeedsAdditionalMove = true;
                                prowlerForAdditionalMove = selectedPiece;
                            }
                            else if (selectedPiece->getType() == "GhostKnight")
                            {
                                if (targetPiece->getType() == "Familiar")
                                {
                                    if (canCapture)
                                    {
                                        ghostKnight = static_cast<GhostKnight *>(selectedPiece);
                                        ghostKnight->stunAdjacentEnemies(targetPosition, pieces, board);
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (selectedPiece->getType() == "PawnHopper")
                            {
                                // Cast selectedPiece to PawnHopper
                                PawnHopper *pawnHopper = static_cast<PawnHopper *>(selectedPiece);

                                // Capture the hopped piece
                                pawnHopper->captureHoppedPiece(targetPosition, pieces);
                            }

                            // Move the selected piece to the target position
                            selectedPiece->setPosition(targetPosition);

                            if (selectedPiece->getType() == "GhostKnight")
                            {
                                ghostKnight = static_cast<GhostKnight *>(selectedPiece);
                                ghostKnight->stunAdjacentEnemies(targetPosition, pieces, board);
                            }
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
                        if (canCapture)
                        {
                            if (!prowlerNeedsAdditionalMove)
                            {
                                // Switch turn
                                isWhiteTurn = !isWhiteTurn;
                            }

                            // Reset the stunned state of all pieces of the current player
                            for (auto &piece : pieces)
                            {
                                if ((isWhiteTurn && piece->getColor() == Piece::Color::Black) ||
                                    (!isWhiteTurn && piece->getColor() == Piece::Color::White))
                                {
                                    piece->setStunned(false);
                                }
                            }
                            turnIndicator.setString(prowlerNeedsAdditionalMove ? "Prowler can move again" : (isWhiteTurn ? "White's Turn" : "Black's Turn"));
                            turnIndicator.setFillColor(sf::Color::White);
                            showTurnIndicator = true;
                            turnIndicatorClock.restart();
                            fadeClock.restart();

                            // Center the turn indicator
                            sf::FloatRect textRect = turnIndicator.getLocalBounds();
                            turnIndicator.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                            turnIndicator.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
                        }
                    }
                }
            }
        }

        // Update the turn indicator text
        turnIndicator.setString(prowlerNeedsAdditionalMove ? "Prowler can move again" : (isWhiteTurn ? "White's Turn" : "Black's Turn"));

        // Code to remove pieces marked for removal
        pieces.erase(std::remove_if(pieces.begin(), pieces.end(),
                                    [](const std::unique_ptr<Piece> &piece)
                                    {
                                        auto hellPawn = dynamic_cast<HellPawn *>(piece.get());
                                        return hellPawn && hellPawn->toBeRemoved;
                                    }),
                     pieces.end());

        // Fade out the turn indicator
        if (showTurnIndicator && turnIndicatorClock.getElapsedTime() > turnIndicatorDisplayTime)
        {
            sf::Time fadeTime = sf::seconds(1); // 1 second fade out
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (1 - fadeClock.getElapsedTime().asSeconds() / fadeTime.asSeconds()));
            turnIndicator.setFillColor(sf::Color(255, 255, 255, alpha));
            if (fadeClock.getElapsedTime() > fadeTime)
            {
                showTurnIndicator = false;
            }
        }

        window.clear();
        drawChessboard(window, board);
        for (const auto &piece : pieces)
        {
            window.draw(piece->getSprite());
        }
        if (showTurnIndicator)
        {
            // Center the turn indicator
            sf::FloatRect textRect = turnIndicator.getLocalBounds();
            turnIndicator.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            turnIndicator.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
            window.draw(turnIndicator);
        }
        window.display();
    }
}