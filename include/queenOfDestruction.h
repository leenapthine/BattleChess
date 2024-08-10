#ifndef QUEENOFDESTRUCTION_H
#define QUEENOFDESTRUCTION_H

#include "piece.h"

class QueenOfDestruction : public Piece
{
public:
    QueenOfDestruction(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void massDestruction(const Piece *capturingPiece, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &boards);

    bool canMoveToIgnoringPiece(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces, const Piece *ignorePiece) const;

    bool isPathClearIgnoringPiece(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces, const Piece *ignorePiece) const;

    std::string getType() const override
    {
        return "QueenOfDestruction";
    }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<QueenOfDestruction>(*this);
    }
};

#endif // QUEENOFDESTRUCTION_H