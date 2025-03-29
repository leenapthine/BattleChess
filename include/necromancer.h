#ifndef NECROMANCER_H
#define NECROMANCER_H

#include "piece.h"
#include "textureManager.h"
#include "globals.h"

class Necromancer : public Piece
{
public:
    Necromancer(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    bool raiseDead(const sf::Vector2f &targetPosition, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager);

    std::string getType() const override { return "Necromancer"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Necromancer>(*this);
    }

    sf::Texture &getPawnTexture(TextureManager &textureManager) const;

private:
    std::vector<sf::Vector2f> getAdjacentPositions(const sf::Vector2f &position) const;
};

#endif
