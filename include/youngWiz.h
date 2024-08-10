#ifndef YOUNGWIZ_H
#define YOUNGWIZ_H

#include "piece.h"

class YoungWiz : public Piece
{
public:
    YoungWiz(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "YoungWiz"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<YoungWiz>(*this);
    }
};

#endif // YOUNGWIZ_H
