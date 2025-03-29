#ifndef BOULDERTHROWER_H
#define BOULDERTHROWER_H

#include "piece.h"
#include <SFML/Graphics.hpp>

class BoulderThrower : public Piece
{
public:
    BoulderThrower(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "BoulderThrower"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<BoulderThrower>(*this);
    }
};

#endif // BOULDERTHROWER_H