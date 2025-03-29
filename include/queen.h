#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece
{
public:
    Queen(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Queen>(*this);
    }

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "Queen"; }
};

#endif // QUEEN_H