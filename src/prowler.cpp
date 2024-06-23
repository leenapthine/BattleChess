// Filename: prowler.cpp

// Description:
// Prowler is a place holder class containing the logic for the Knight Piece.
// Prowler is a Level 2 Knight piece belonging to the Hellspawn race.
// The piece logic will eventually allow for the Prowler to move again after capturing an enemy piece.

#include "piece.h"
#include "prowler.h"
#include "utility.h"
#include "globals.h"
#include "square.h"
#include <vector>

bool Prowler::canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const
{
    sf::Vector2f currentPosition = getPosition();
    int dx = std::abs(target.x - currentPosition.x) / TILE_SIZE;
    int dy = std::abs(target.y - currentPosition.y) / TILE_SIZE;

    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
    {
        return !getPieceAtPosition(target, pieces) || isOpponentPiece(target, pieces, getColor());
    }
    return false;
}

void Prowler::highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const
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
    }
};