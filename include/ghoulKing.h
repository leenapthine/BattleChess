#ifndef GHOULKING_H
#define GHOULKING_H

#include "piece.h"
#include "textureManager.h"

class GhoulKing : public Piece
{
public:
    GhoulKing(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::vector<sf::Vector2f> getPerimeterPositions(const sf::Vector2f &position) const;

    sf::Texture &getNecroPawnTexture(TextureManager &textureManager) const;

    bool raiseNecroPawn(const sf::Vector2f &targetPosition, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager);

    std::string getType() const override { return "GhoulKing"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<GhoulKing>(*this);
    }

    mutable bool raisedDead = false;
};

#endif // GHOULKING_H