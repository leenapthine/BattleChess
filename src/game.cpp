#include "game.h"
#include "textureManager.h"
#include "piece.h"
#include "wizard.h"
#include "youngWiz.h"
#include "necromancer.h"
#include "queenOfDomination.h"
#include "queenOfDestruction.h"
#include "pawn.h"
#include "pawnHopper.h"
#include "ghoulKing.h"
#include "necroPawn.h"
#include "hellPawn.h"
#include "hellKing.h"
#include "queenOfIllusions.h"
#include "queenOfBones.h"
#include "boulderThrower.h"
#include "beholder.h"
#include "deadLauncher.h"
#include "howler.h"
#include "prowler.h"
#include "portal.h"
#include "familiar.h"
#include "ghostKnight.h"
#include "wizardKing.h"
#include "utility.h"
#include "globals.h"
#include "piece.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>

void clearSelection(std::vector<std::vector<Square>> &board)
{
    for (auto &row : board)
    {
        for (auto &square : row)
        {
            square.setHighlight(false);
        }
    }
}

Piece *playerSelectPawn(sf::RenderWindow &window, const std::vector<Piece *> &sacrificablePawns, std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces, QueenOfBones &queen)
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);

                    // Check if the mouse click is on a sacrificable pawn
                    for (auto pawn : sacrificablePawns)
                    {
                        if (pawn->getSprite().getGlobalBounds().contains(worldPosition))
                        {
                            // Get the position of the pawn
                            sf::Vector2f piecePosition = pawn->getPosition();
                            int x = static_cast<int>(piecePosition.x / TILE_SIZE);
                            int y = static_cast<int>(piecePosition.y / TILE_SIZE);

                            // Highlight the corresponding square on the board
                            board[y][x].setHighlight(true, sf::Color::Red);

                            window.clear();
                            drawChessboard(window, board);

                            for (const auto &piece : pieces)
                            {
                                window.draw(piece->getSprite());
                            }

                            window.display();

                            return pawn; // Return the selected pawn
                        }
                        else if (queen.getSprite().getGlobalBounds().contains(worldPosition))
                        {
                            queen.pawnsToSacrifice = -1;
                            return nullptr;
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

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
    TextureManager textureManager;
    std::vector<std::unique_ptr<Piece>> pieces;
    try
    {
        loadTextures(textureManager);         // Load textures for all pieces
        createPieces(textureManager, pieces); // Create and add pieces
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

    // Initialize deadLauncher variable
    DeadLauncher *deadLauncher = nullptr;

    // Initialize QueenOfIllusions variable
    QueenOfIllusions *queenOfIllusions = nullptr;

    // Initialize QueenOfDomination variable
    QueenOfDomination *queenOfDomination = nullptr;

    // Initialize GhoulKing variable
    bool awaitingNecroPawnPlacement = false;
    GhoulKing *ghoulKing = nullptr;

    // Initialize portal/portals variables
    Portal *portal = nullptr;
    std::vector<Portal *> portals;
    for (const auto &piece : pieces)
    {
        if (piece->getType() == "Portal")
        {
            portals.push_back(static_cast<Portal *>(piece.get()));
        }
    }

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
                        pushPawn(pieces, necromancer->getPawnTexture(textureManager), selectedPiecePosition, necromancer->getColor());
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

                if (awaitingNecroPawnPlacement)
                {
                    if (board[row][col].getHighlight())
                    {
                        pushNecroPawn(pieces, ghoulKing->getNecroPawnTexture(textureManager), selectedPiecePosition, ghoulKing->getColor());
                        awaitingNecroPawnPlacement = false;
                        ghoulKing = nullptr;
                        clearSelection(board);
                        continue;
                    }
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
                    // Piece is selected and its movement is highlighted
                    else if (selectedPiece && board[row][col].getHighlight())
                    {
                        sf::Vector2f targetPosition = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);
                        Piece *targetPiece = getPieceAtPosition(targetPosition, pieces);

                        // a piece is clicked and it is not the selected piece
                        if (targetPiece && targetPiece != selectedPiece)
                        {
                            // Check if the piece is a QueenOfBones
                            if (targetPiece->getType() == "QueenOfBones")
                            {
                                auto queen = static_cast<QueenOfBones *>(targetPiece);
                                // Call the revive method if the piece is a QueenOfBones
                                std::vector<Piece *> sacrificablePawns = queen->revive(pieces);

                                // If there are not enough pawns to sacrifice, the revive cannot occur
                                if (sacrificablePawns.size() < 2)
                                {
                                    std::cout << "Not enough pawns to sacrifice. Revive failed." << std::endl;
                                }
                                else
                                {
                                    std::cout << "REVIVE QUEENOFBONES?: Select two pawns to sacrifice. ELSE: Select QueenOfBones." << std::endl;

                                    while (queen->pawnsToSacrifice > 0)
                                    {
                                        // Implement a mechanism to allow player to select a pawn from the sacrificablePawns vector
                                        Piece *selectedPawn = playerSelectPawn(window, sacrificablePawns, board, pieces, *queen);

                                        if (selectedPawn != nullptr)
                                        {
                                            queen->handlePawnSacrifice(selectedPawn, pieces); // Pass the selected pawn for sacrifice
                                        }
                                    }
                                    if (queen->pawnsToSacrifice == 0)
                                    {
                                        // Attempt to respawn the QueenOfBones
                                        queen->respawnAtOriginalPosition(pieces, textureManager);
                                    }

                                    // Reset pawnsToSacrifice
                                    queen->pawnsToSacrifice = 2;
                                }
                            }
                            // Capture the clicked piece
                            selectedPiece->capture(targetPosition, pieces);
                        }

                        // Check it killed piece if QueenOfDestruction (activate massDestruction)
                        else if (targetPiece->getType() == "QueenOfDestruction")
                        {
                            auto queen = static_cast<QueenOfDestruction *>(targetPiece);
                            queen->massDestruction(selectedPiece, pieces, board);
                        }

                        else
                        {
                            // and empty highlighted square is selected, move there.
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

                        // Reset the stunned state of all pieces of the current player
                        for (auto &piece : pieces)
                        {
                            if ((isWhiteTurn && piece->getColor() == Piece::Color::Black) ||
                                (!isWhiteTurn && piece->getColor() == Piece::Color::White))
                            {
                                piece->setStunned(false);
                            }
                        }

                        for (auto &piece : pieces)
                        {
                            auto queenOfDomination = dynamic_cast<QueenOfDomination *>(piece.get());
                            if (queenOfDomination && queenOfDomination->originalType != "QueenOfDomination")
                            {
                                queenOfDomination->returnOriginalSprite(pieces, textureManager);
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
                    // a piece has not been clicked on twice
                    if (!pieceSelected)
                    {
                        pieceSelected = true;
                        selectedPiece = clickedPiece;

                        if (selectedPiece->isStunned())
                        {
                            // Provide feedback that the piece is stunned
                            std::cout << "Selected piece is stunned and cannot move this turn." << std::endl;
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

                            // Highlighting moves for all pieces happens here
                            selectedPiece->highlightValidMoves(board, pieces);
                        }

                        // Highlight capture zones for BoulderThrower
                        if (selectedPiece->getType() == "BoulderThrower")
                        {
                            auto &boulderThrower = static_cast<BoulderThrower &>(*selectedPiece);
                            boulderThrower.highlightCaptureZones(board, pieces);
                        }

                        // Highlight capture zones for Beholder
                        if (selectedPiece->getType() == "Beholder")
                        {
                            auto &beholder = static_cast<Beholder &>(*selectedPiece);
                            beholder.highlightCaptureZones(board, pieces);
                        }

                        // Highlight capture zones for HellKing
                        if (selectedPiece->getType() == "HellKing")
                        {
                            auto &hellKing = static_cast<HellKing &>(*selectedPiece);
                            hellKing.highlightCaptureZones(board, pieces);
                        }

                        // Highlight capture zones for deadLauncher
                        if (selectedPiece->getType() == "DeadLauncher")
                        {
                            DeadLauncher *deadLauncher = static_cast<DeadLauncher *>(selectedPiece);
                            // Highlight adjacent Pawns/NecroPawns
                            deadLauncher->highlightAdjacentPawns(pieces, board);
                            if (deadLauncher->pawnLoaded == true)
                            {
                                deadLauncher->highlightDeadCaptureZones(board, pieces);
                            }
                        }

                        // Highlight capture zones for WizardKing
                        if (selectedPiece->getType() == "WizardKing")
                        {
                            WizardKing *wizardKing = static_cast<WizardKing *>(selectedPiece);
                            wizardKing->highlightCaptureZones(board, pieces);
                        }

                        // Highlight pieces able to load
                        if (selectedPiece->getType() == "Portal")
                        {
                            Portal *portal = static_cast<Portal *>(selectedPiece);
                            // Highlight adjacent Pieces
                            portal->highlightAdjacentPieces(pieces, board, isWhiteTurn);
                        }
                    }
                    else if (selectedPiece == clickedPiece)
                    {
                        if (selectedPiece->getType() == "NecroPawn" && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black))
                        {
                            // Check if it's the turn of the selected piece's color
                            if ((selectedPiece->getColor() == Piece::Color::White && isWhiteTurn) ||
                                (selectedPiece->getColor() == Piece::Color::Black && !isWhiteTurn))
                            {
                                auto &necroPawn = static_cast<NecroPawn &>(*selectedPiece);

                                sf::Vector2f currentPosition = necroPawn.getPosition();
                                std::vector<sf::Vector2f> capturePositions = necroPawn.getCapturePositions();

                                // Iterate over all adjacent positions and capture any pieces found
                                for (const auto &pos : capturePositions)
                                {
                                    for (auto it = pieces.begin(); it != pieces.end();)
                                    {
                                        Piece *piece = it->get(); // Access the raw pointer from std::unique_ptr
                                        if (piece && piece->getPosition() == pos)
                                        {
                                            // Check if the piece is a QueenOfBones
                                            if (piece->getType() == "QueenOfBones")
                                            {
                                                auto queen = static_cast<QueenOfBones *>(piece);
                                                // Call the revive method if the piece is a QueenOfBones
                                                std::vector<Piece *> sacrificablePawns = queen->revive(pieces);

                                                // If there are not enough pawns to sacrifice, the revive cannot occur
                                                if (sacrificablePawns.size() < 2)
                                                {
                                                    std::cout << "Not enough pawns to sacrifice. Revive failed." << std::endl;
                                                }
                                                else
                                                {
                                                    std::cout << "REVIVE QUEENOFBONES?: Select two pawns to sacrifice. ELSE: Select QueenOfBones." << std::endl;

                                                    while (queen->pawnsToSacrifice > 0)
                                                    {
                                                        // Implement a mechanism to allow player to select a pawn from the sacrificablePawns vector
                                                        Piece *selectedPawn = playerSelectPawn(window, sacrificablePawns, board, pieces, *queen);

                                                        if (selectedPawn != nullptr)
                                                        {
                                                            queen->handlePawnSacrifice(selectedPawn, pieces); // Pass the selected pawn for sacrifice
                                                        }
                                                    }

                                                    if (queen->pawnsToSacrifice == 0)
                                                    {
                                                        // Attempt to respawn the QueenOfBones
                                                        queen->respawnAtOriginalPosition(pieces, textureManager);
                                                    }

                                                    // Reset pawnsToSacrifice
                                                    queen->pawnsToSacrifice = 2;
                                                }
                                                break;
                                            }

                                            // Check it killed piece if QueenOfDestruction (activate massDestruction)
                                            else if (piece->getType() == "QueenOfDestruction")
                                            {
                                                auto queen = static_cast<QueenOfDestruction *>(piece);
                                                queen->massDestruction(selectedPiece, pieces, board);
                                            }

                                            // Erase the captured piece
                                            if (Piece *piece = it->get())
                                            {
                                                it = pieces.erase(it); // Properly erase and reassign iterator
                                            }
                                        }
                                        else
                                        {
                                            ++it; // Increment iterator only when not erasing
                                        }
                                    }
                                }

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

                                for (auto &piece : pieces)
                                {
                                    auto queenOfDomination = dynamic_cast<QueenOfDomination *>(piece.get());
                                    if (queenOfDomination && queenOfDomination->originalType != "QueenOfDomination")
                                    {
                                        queenOfDomination->returnOriginalSprite(pieces, textureManager);
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
                        else if (selectedPiece->getType() == "QueenOfDomination" && !dynamic_cast<QueenOfDomination *>(selectedPiece)->hasUsedAbility())
                        {
                            auto queen = dynamic_cast<QueenOfDomination *>(selectedPiece);

                            // First, check if any non-highlighted friendly unit is clicked
                            bool clickedNonHighlighted = false;

                            for (auto &row : board)
                            {
                                for (auto &square : row)
                                {
                                    // Simplified comparison by directly converting Piece::Color to sf::Color
                                    sf::Color pieceColor = (selectedPiece->getColor() == Piece::Color::White) ? sf::Color::White : sf::Color::Black;

                                    if (!square.getHighlight() && pieceColor == square.getShape().getFillColor())
                                    {
                                        clickedNonHighlighted = true;
                                        break; // Exit the loop if a non-highlighted square is found
                                    }
                                }

                                if (clickedNonHighlighted)
                                {
                                    break; // Exit the outer loop if condition is met
                                }
                            }

                            if (clickedNonHighlighted)
                            {
                                // Clear highlights if clicking on a non-highlighted friendly unit
                                queen->clearHighlights(board);

                                // Reset selection state
                                selectedPiece = nullptr;
                                std::cout << "Highlights cleared. Piece deselected." << std::endl;
                            }
                            else
                            {
                                // Toggle highlights when clicking the QueenOfDomination again
                                queen->toggleAdjacentFriendlyHighlights(board, pieces);
                            }
                        }

                        // Familiar turns to stone
                        else if (selectedPiece->getType() == "Familiar" &&
                                     (isWhiteTurn && selectedPiece->getColor() == Piece::Color::White) ||
                                 selectedPiece->getType() == "Familiar" &&
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

                            for (auto &piece : pieces)
                            {
                                auto queenOfDomination = dynamic_cast<QueenOfDomination *>(piece.get());
                                if (queenOfDomination && queenOfDomination->originalType != "QueenOfDomination")
                                {
                                    queenOfDomination->returnOriginalSprite(pieces, textureManager);
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

                        // Portal actions (unload)
                        else if (selectedPiece->getType() == "Portal" && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black) &&
                                 static_cast<Portal *>(selectedPiece)->pieceLoaded)
                        {
                            Portal *portal = static_cast<Portal *>(selectedPiece);
                            sf::Vector2f portalPosition = portal->getPosition();

                            if (portal->pieceLoaded)
                            {
                                // Set unload highlights
                                portal->teleport(window, board, pieces, isWhiteTurn, portals);
                                portal->highlightValidMoves(board, pieces);
                            }
                        }

                        else if (selectedPiece->getType() == "GhoulKing" && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black) &&
                                 static_cast<GhoulKing *>(selectedPiece)->raisedDead == false)
                        {
                            ghoulKing = static_cast<GhoulKing *>(selectedPiece);
                            sf::Vector2f ghoulPos = selectedPiece->getPosition();
                            awaitingNecroPawnPlacement = dynamic_cast<GhoulKing *>(selectedPiece)->raiseNecroPawn(ghoulPos, board, pieces, textureManager);
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
                            if (selectedPiece->getType() == "DeadLauncher" && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black))
                            {
                                DeadLauncher *deadLauncher = static_cast<DeadLauncher *>(selectedPiece);
                                if (clickedPiece->getType() == "Pawn" || clickedPiece->getType() == "NecroPawn")
                                {
                                    sf::Vector2f clickedPiecePosition = clickedPiece->getPosition();
                                    sf::Vector2f deadLauncherPosition = deadLauncher->getPosition();

                                    // Check if the clicked piece is adjacent to the DeadLauncher
                                    int col = deadLauncherPosition.x / TILE_SIZE;
                                    int row = deadLauncherPosition.y / TILE_SIZE;

                                    for (const auto &dir : deadLauncher->directions)
                                    {
                                        int newRow = row + dir[0];
                                        int newCol = col + dir[1];

                                        // Check if the clicked piece's position matches any adjacent position
                                        if (clickedPiecePosition == sf::Vector2f(newCol * TILE_SIZE, newRow * TILE_SIZE))
                                        {
                                            if (deadLauncher->pawnLoaded == false)
                                            {
                                                deadLauncher->capture(clickedPiecePosition, pieces);
                                                deadLauncher->pawnLoaded = true;

                                                // Clear all highlights after turned to stone
                                                for (int r = 0; r < BOARD_SIZE; ++r)
                                                {
                                                    for (int c = 0; c < BOARD_SIZE; ++c)
                                                    {
                                                        board[r][c].setHighlight(false);
                                                    }
                                                }

                                                deadLauncher->highlightValidMoves(board, pieces);
                                            }
                                            else
                                            {
                                                std::cout << "Pawn is already loaded." << std::endl;
                                            }
                                        }
                                    }
                                }
                            }
                            else if (selectedPiece->getType() == "QueenOfDomination" && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black))
                            {
                                // Check if the clicked position corresponds to a highlighted square
                                bool isHighlighted = false;
                                const sf::Vector2f clickedPosition = clickedPiece->getPosition();
                                auto queen = dynamic_cast<QueenOfDomination *>(selectedPiece);

                                for (const auto &row : board)
                                {
                                    for (const auto &square : row)
                                    {
                                        if (square.getPosition() == clickedPosition)
                                        {
                                            if (square.getHighlight())
                                            {
                                                isHighlighted = true;
                                                break; // Breaks out of the inner loop
                                            }
                                            else
                                            {
                                                queen->clearHighlights(board);
                                            }
                                        }
                                    }
                                }

                                if (isHighlighted && !queen->hasUsedAbility())
                                {
                                    queen->applyDominationAbility(clickedPiece, pieces, textureManager);

                                    // Clear highlights
                                    clearSelection(board);
                                }

                                pieceSelected = false;
                            }

                            else if (selectedPiece->getType() == "Portal" && selectedPiece->getColor() == (isWhiteTurn ? Piece::Color::White : Piece::Color::Black) &&
                                     static_cast<Portal *>(selectedPiece)->pieceLoaded == false)
                            {
                                Portal *portal = static_cast<Portal *>(selectedPiece);
                                sf::Vector2f clickedPiecePosition = clickedPiece->getPosition();
                                sf::Vector2f portalPosition = portal->getPosition();

                                // Check if the clicked piece is adjacent to the Portal
                                int col = portalPosition.x / TILE_SIZE;
                                int row = portalPosition.y / TILE_SIZE;

                                for (const auto &dir : portal->directions)
                                {
                                    int newRow = row + dir[0];
                                    int newCol = col + dir[1];

                                    // Check if the clicked piece's position matches any adjacent position
                                    if (clickedPiecePosition == sf::Vector2f(newCol * TILE_SIZE, newRow * TILE_SIZE))
                                    {
                                        if (!portal->pieceLoaded)
                                        {
                                            portal->captureWithPortals(clickedPiecePosition, pieces, portals, board);
                                            std::cout << "Piece is loaded into the portal!" << std::endl;

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
                                            std::cout << "Piece is already loaded." << std::endl;
                                        }
                                        portal->highlightValidMoves(board, pieces);
                                        break;
                                    }
                                }
                            }
                            else if (selectedPiece->getType() == "QueenOfIllusions" &&
                                     (clickedPiece->getType() == "Pawn" || clickedPiece->getType() == "YoungWiz"))
                            {
                                QueenOfIllusions *queenOfIllusions = static_cast<QueenOfIllusions *>(selectedPiece);

                                if ((selectedPiece->getColor() == Piece::Color::White && isWhiteTurn) ||
                                    (selectedPiece->getColor() == Piece::Color::Black && !isWhiteTurn))
                                {
                                    queenOfIllusions->swap(clickedPiece, board);

                                    // deselect pieceselected
                                    pieceSelected = false;

                                    // Switch turn after swapping
                                    isWhiteTurn = !isWhiteTurn;

                                    for (auto &piece : pieces)
                                    {
                                        auto queenOfDomination = dynamic_cast<QueenOfDomination *>(piece.get());
                                        if (queenOfDomination && queenOfDomination->originalType != "QueenOfDomination")
                                        {
                                            queenOfDomination->returnOriginalSprite(pieces, textureManager);
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
                            }
                            else
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

                                    // THIS IS WHERE MOVEMENT HAPPENS
                                    if (canCapture)
                                    {
                                        // Check if the piece is a QueenOfBones
                                        if (targetPiece->getType() == "QueenOfBones")
                                        {
                                            auto queen = static_cast<QueenOfBones *>(targetPiece);
                                            // Call the revive method if the piece is a QueenOfBones
                                            std::vector<Piece *> sacrificablePawns = queen->revive(pieces);

                                            // If there are not enough pawns to sacrifice, the revive cannot occur
                                            if (sacrificablePawns.size() < 2)
                                            {
                                                std::cout << "Not enough pawns to sacrifice. Revive failed." << std::endl;
                                            }
                                            else
                                            {
                                                std::cout << "REVIVE QUEENOFBONES?: Select two pawns to sacrifice. ELSE: Select QueenOfBones." << std::endl;

                                                while (queen->pawnsToSacrifice > 0)
                                                {
                                                    // Implement a mechanism to allow player to select a pawn from the sacrificablePawns vector
                                                    Piece *selectedPawn = playerSelectPawn(window, sacrificablePawns, board, pieces, *queen);

                                                    if (selectedPawn != nullptr)
                                                    {
                                                        queen->handlePawnSacrifice(selectedPawn, pieces); // Pass the selected pawn for sacrifice
                                                    }
                                                }
                                                if (queen->pawnsToSacrifice == 0)
                                                {
                                                    // Attempt to respawn the QueenOfBones
                                                    queen->respawnAtOriginalPosition(pieces, textureManager);
                                                }

                                                // Reset pawnsToSacrifice
                                                queen->pawnsToSacrifice = 2;
                                            }
                                        }

                                        // Check it killed piece if QueenOfDestruction (activate massDestruction)
                                        else if (targetPiece->getType() == "QueenOfDestruction" && selectedPiece->getType() != "HellPawn")
                                        {
                                            auto queen = static_cast<QueenOfDestruction *>(targetPiece);
                                            queen->massDestruction(selectedPiece, pieces, board);
                                        }

                                        else if (selectedPiece->getType() == "HellPawn")
                                        {
                                            auto hellPawn = static_cast<HellPawn *>(selectedPiece);
                                            hellPawn->infect(targetPiece, pieces, board, textureManager);
                                        }

                                        // PERFORM CAPTURE !!!!!!!
                                        if (selectedPiece->getType() != "HellPawn")
                                        {
                                            selectedPiece->capture(targetPosition, pieces); // Capture the target piece
                                        }
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
                                        awaitingPawnPlacement = necromancer->raiseDead(targetPosition, board, pieces, textureManager);
                                    }
                                    else if (selectedPiece->getType() == "Prowler")
                                    {
                                        prowlerNeedsAdditionalMove = true;
                                        prowlerForAdditionalMove = selectedPiece;
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

                                    for (auto &piece : pieces)
                                    {
                                        auto queenOfDomination = dynamic_cast<QueenOfDomination *>(piece.get());
                                        if (queenOfDomination && queenOfDomination->originalType != "QueenOfDomination")
                                        {
                                            queenOfDomination->returnOriginalSprite(pieces, textureManager);
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
                else if (pieceSelected && awaitingNecroPawnPlacement == false)
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
                                // Check if the piece is a QueenOfBones
                                if (targetPiece->getType() == "QueenOfBones")
                                {
                                    auto queen = static_cast<QueenOfBones *>(targetPiece);
                                    // Call the revive method if the piece is a QueenOfBones
                                    std::vector<Piece *> sacrificablePawns = queen->revive(pieces);

                                    // If there are not enough pawns to sacrifice, the revive cannot occur
                                    if (sacrificablePawns.size() < 2)
                                    {
                                        std::cout << "Not enough pawns to sacrifice. Revive failed." << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "REVIVE QUEENOFBONES?: Select two pawns to sacrifice. ELSE: Select QueenOfBones." << std::endl;

                                        while (queen->pawnsToSacrifice > 0)
                                        {
                                            // Implement a mechanism to allow player to select a pawn from the sacrificablePawns vector
                                            Piece *selectedPawn = playerSelectPawn(window, sacrificablePawns, board, pieces, *queen);

                                            if (selectedPawn != nullptr)
                                            {
                                                queen->handlePawnSacrifice(selectedPawn, pieces); // Pass the selected pawn for sacrifice
                                            }
                                        }
                                        if (queen->pawnsToSacrifice == 0)
                                        {
                                            // Attempt to respawn the QueenOfBones
                                            queen->respawnAtOriginalPosition(pieces, textureManager);
                                        }

                                        // Reset pawnsToSacrifice
                                        queen->pawnsToSacrifice = 2;
                                    }
                                }

                                // Check it killed piece if QueenOfDestruction (activate massDestruction)
                                else if (targetPiece->getType() == "QueenOfDestruction")
                                {
                                    auto queen = static_cast<QueenOfDestruction *>(targetPiece);
                                    queen->massDestruction(selectedPiece, pieces, board);
                                }

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
                                awaitingPawnPlacement = necromancer->raiseDead(targetPosition, board, pieces, textureManager);
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

                                sf::Vector2f currentPosition = pawnHopper->getPosition();
                                float direction = (pawnHopper->getColor() == Piece::Color::White) ? 1.0f : -1.0f;

                                if (targetPosition.x == currentPosition.x && std::abs(targetPosition.y - currentPosition.y) == 2 * TILE_SIZE)
                                {
                                    sf::Vector2f hoppedPosition = sf::Vector2f(currentPosition.x, currentPosition.y + direction * TILE_SIZE);
                                    for (const auto &piecePtr : pieces)
                                    {
                                        Piece *piece = piecePtr.get(); // Access the raw pointer from std::unique_ptr

                                        if (piece->getPosition() == hoppedPosition && piece->getColor() != pawnHopper->getColor())
                                        {
                                            // Check if the piece is a QueenOfBones
                                            if (piece->getType() == "QueenOfBones")
                                            {
                                                auto queen = static_cast<QueenOfBones *>(piece);
                                                // Call the revive method if the piece is a QueenOfBones
                                                std::vector<Piece *> sacrificablePawns = queen->revive(pieces);

                                                // If there are not enough pawns to sacrifice, the revive cannot occur
                                                if (sacrificablePawns.size() < 2)
                                                {
                                                    std::cout << "Not enough pawns to sacrifice. Revive failed." << std::endl;
                                                }
                                                else
                                                {
                                                    std::cout << "REVIVE QUEENOFBONES?: Select two pawns to sacrifice. ELSE: Select QueenOfBones." << std::endl;

                                                    while (queen->pawnsToSacrifice > 0)
                                                    {
                                                        // Implement a mechanism to allow player to select a pawn from the sacrificablePawns vector
                                                        Piece *selectedPawn = playerSelectPawn(window, sacrificablePawns, board, pieces, *queen);

                                                        if (selectedPawn != nullptr)
                                                        {
                                                            queen->handlePawnSacrifice(selectedPawn, pieces); // Pass the selected pawn for sacrifice
                                                        }
                                                    }
                                                    if (queen->pawnsToSacrifice == 0)
                                                    {
                                                        // Attempt to respawn the QueenOfBones
                                                        queen->respawnAtOriginalPosition(pieces, textureManager);
                                                    }

                                                    // Reset pawnsToSacrifice
                                                    queen->pawnsToSacrifice = 2;
                                                    break;
                                                }
                                            }

                                            // Check it killed piece if QueenOfDestruction (activate massDestruction)
                                            else if (piece->getType() == "QueenOfDestruction")
                                            {
                                                auto queen = static_cast<QueenOfDestruction *>(piece);
                                                queen->massDestruction(selectedPiece, pieces, board);
                                            }
                                        }
                                    }
                                    // Capture the hopped piece
                                    pawnHopper->captureHoppedPiece(targetPosition, pieces);
                                }
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
                            for (auto &piece : pieces)
                            {
                                auto queenOfDomination = dynamic_cast<QueenOfDomination *>(piece.get());
                                if (queenOfDomination && queenOfDomination->originalType != "QueenOfDomination")
                                {
                                    queenOfDomination->returnOriginalSprite(pieces, textureManager);
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