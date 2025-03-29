#ifndef ROOK_H
#define ROOK_H

#include "piece.h"
#include <SFML/Graphics.hpp>

class Rook : public Piece
{
public:
    Rook(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "Rook"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Rook>(*this);
    }
};

#endif // ROOK_H
