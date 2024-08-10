#ifndef FROGKING_H
#define FROGKING_H

#include "piece.h"

class FrogKing : public Piece
{
public:
    FrogKing(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "FrogKing"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<FrogKing>(*this);
    }
};

#endif // FROGKING_H