#ifndef BEASTDRUID_H
#define BEASTDRUID_H

#include "piece.h"

class BeastDruid : public Piece
{
public:
    BeastDruid(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<BeastDruid>(*this);
    }

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "BeastDruid"; }
};

#endif // BEASTDRUID_H