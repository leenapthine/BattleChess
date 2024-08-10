#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece
{
public:
    King(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "King"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<King>(*this);
    }
};

#endif // KING_H