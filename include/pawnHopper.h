#ifndef PAWNHOPPER_H
#define PAWNHOPPER_H

#include "piece.h"

class PawnHopper : public Piece
{
public:
    PawnHopper(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void captureHoppedPiece(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) const;

    std::string getType() const override { return "PawnHopper"; }
};

#endif // PAWNHOPPER_H