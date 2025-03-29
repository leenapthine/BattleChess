#ifndef QUEENOFILLUSIONS_H
#define QUEENOFILLUSIONS_H

#include "piece.h"

class QueenOfIllusions : public Piece
{
public:
    QueenOfIllusions(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void swap(Piece *targetPiece, std::vector<std::vector<Square>> &board);

    std::string getType() const override { return "QueenOfIllusions"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<QueenOfIllusions>(*this);
    }
};

#endif // QUEENOFILLUSIONS_H