#ifndef NECROPAWN_H
#define NECROPAWN_H

#include "piece.h"

class NecroPawn : public Piece
{
public:
    NecroPawn(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void sacrifice(std::vector<std::unique_ptr<Piece>> &pieces);

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "NecroPawn"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<NecroPawn>(*this);
    }

    std::vector<sf::Vector2f> getCapturePositions() const;
};

#endif // NECROPAWN_H
