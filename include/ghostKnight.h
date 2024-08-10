#ifndef GHOSTKNIGHT_H
#define GHOSTKNIGHT_H

#include "piece.h"

class GhostKnight : public Piece
{
public:
    GhostKnight(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void stunAdjacentEnemies(const sf::Vector2f &position, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board); // Stun adjacent enemies

    std::string getType() const override { return "GhostKnight"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<GhostKnight>(*this);
    }
};

#endif // GHOSTKNIGHT_H