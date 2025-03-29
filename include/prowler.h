#ifndef PROWLER_H
#define PROWLER_H

#include "piece.h"

class Prowler : public Piece
{
public:
    Prowler(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "Prowler"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Prowler>(*this);
    }

    bool needsAdditionalMove; // Flag to track if the Prowler needs an additional move
};

#endif // PROWLER_H