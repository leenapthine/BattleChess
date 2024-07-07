#ifndef FAMILIAR_H
#define FAMILIAR_H

#include "piece.h"

class Familiar : public Piece
{
public:
    Familiar(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    bool isStone() const { return stone; }
    void setStone(bool value);

    std::string getType() const override { return "Familiar"; }

private:
    bool stone = false;
};

#endif // FAMILIAR_H