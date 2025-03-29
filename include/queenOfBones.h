#ifndef QUEENOFBONES_H
#define QUEENOFBONES_H

#include "piece.h"
#include "textureManager.h"

class QueenOfBones : public Piece
{
public:
    QueenOfBones(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::vector<Piece *> revive(std::vector<std::unique_ptr<Piece>> &pieces);

    void respawnAtOriginalPosition(std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager);

    void handlePawnSacrifice(Piece *selectedPawn, std::vector<std::unique_ptr<Piece>> &pieces);

    int pawnsToSacrifice = 2;

    std::string getType() const override
    {
        return "QueenOfBones";
    }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<QueenOfBones>(*this);
    }
};

#endif // QUEENOFBONES_H