#ifndef BEASTKNIGHT_H
#define BEASTKNIGHT_H

#include "piece.h"

class BeastKnight : public Piece
{
public:
    BeastKnight(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<BeastKnight>(*this);
    }

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "BeastKnight"; }
};

#endif // BEASTKNIGHT_H