#ifndef HELLKING_H
#define HELLKING_H

#include "piece.h"

class HellKing : public Piece
{
public:
    HellKing(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "HellKing"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<HellKing>(*this);
    }
};

#endif // HELLKING_H