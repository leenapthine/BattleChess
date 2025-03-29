#ifndef WIZARD_H
#define WIZARD_H

#include "piece.h"

class Wizard : public Piece
{
public:
    Wizard(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "Wizard"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Wizard>(*this);
    }
};

#endif