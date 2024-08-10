// Filename: GhoulKing.cpp
// Description: Implementation of the GhoulKing class, which defines the behavior and movement rules of the GhoulKing piece in our chess game.

// Main Classes:
// - GhoulKing: Represents the Pawn piece and manages its movement options, highlighting, and capture abilities on the chessboard.

// Main Functions:
// - bool GhoulKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Checks if the GhoulKing can legally move to the specified target position on the chessboard.
// - void GhoulKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const:
//   Highlights all valid moves for the GhoulKing on the provided chessboard, considering its current position and game rules.
// - void GhoulKing::RaiseNecroPawn()

// Special Features or Notes:
// - GhoulKing is a level 2 King piece belonging to the Necromancer Guild.
// - The GhoulKing moves one square in any direction.
// - It captures on movement into a space occupied by an opponent piece.
// - Has a special abiliy: RaiseNecroPawn -> once per game before moving GhoulKing
//   can raise a necroPawn anyhere in the perimeter of the GhoulKing.
// - Currently does not contain logic for the King / Rook 'castling' function.
// - Inherits functionality from the Piece class.

// Usage or Context:
// - This file implements the movement and interaction logic specific to the GhoulKing piece.

#include "piece.h"
#include "ghoulKing.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

// Determine if the GhoulKing can move to a specified target position
bool GhoulKing::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
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

// Highlight valid moves for the GhoulKing on the provided board
void GhoulKing::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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

// Place a pawn piece in a selected adjacent square
bool GhoulKing::raiseNecroPawn(const sf::Vector2f &targetPosition, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager)
{
    raisedDead = true;
    bool awaitingNecroPawnPlacement = false;
    const sf::Texture &pawnTexture = getNecroPawnTexture(textureManager);

    // Get all adjacent positions to the target position
    std::vector<sf::Vector2f> possiblePositions = getPerimeterPositions(targetPosition);

    for (const auto &pos : possiblePositions)
    {
        // Check if the adjacent position is within board bounds
        if (pos.x >= 0 && pos.x < BOARD_SIZE * TILE_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE * TILE_SIZE)
        {
            // Check if the adjacent position is empty
            if (!getPieceAtPosition(pos, pieces))
            {
                awaitingNecroPawnPlacement = true;
                std::cout << "Made it to after awaitingNecroPawnPlacement: " << awaitingNecroPawnPlacement << std::endl;
                int col = pos.x / TILE_SIZE;
                int row = pos.y / TILE_SIZE;
                // Highlight the square for pawn placement
                board[row][col].setHighlight(true, sf::Color::Green);
            }
        }
    }
    return awaitingNecroPawnPlacement;
}

// Return the appropriate pawn texture based on the Necromancer's color
sf::Texture &GhoulKing::getNecroPawnTexture(TextureManager &textureManager) const
{
    // Determine the texture name based on the ghoulKing's color
    std::string textureName = (getColor() == Piece::Color::White) ? "WhiteNecroPawn" : "BlackNecroPawn";
    // std::string textureName = "WhiteNecroPawn";

    // Retrieve the texture from the TextureManager
    sf::Texture *texture = textureManager.getTexture(textureName);

    // Handle the case where the texture is not found
    if (texture == nullptr)
    {
        throw std::runtime_error("Texture not found: " + textureName);
    }

    return *texture;
}

// Return all four adjacent positions (left, right, up, down)
std::vector<sf::Vector2f> GhoulKing::getPerimeterPositions(const sf::Vector2f &position) const
{
    return {
        {position.x + TILE_SIZE, position.y},
        {position.x - TILE_SIZE, position.y},
        {position.x, position.y + TILE_SIZE},
        {position.x, position.y - TILE_SIZE},
        {position.x + TILE_SIZE, position.y + TILE_SIZE},
        {position.x - TILE_SIZE, position.y - TILE_SIZE},
        {position.x - TILE_SIZE, position.y + TILE_SIZE},
        {position.x + TILE_SIZE, position.y - TILE_SIZE}};
};