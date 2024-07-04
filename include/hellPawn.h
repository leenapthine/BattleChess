#ifndef HELLPAWN_H
#define HELLPAWN_H

#include "piece.h"

class HellPawn : public Piece
{
public:
    HellPawn(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "HellPawn"; }

    sf::Texture &getRookTexture() const;
    sf::Texture &getKnightTexture() const;
    sf::Texture &getBishopTexture() const;
    sf::Texture &getQueenTexture() const;
    sf::Texture &getBeastKnightTexture() const;
    sf::Texture &getBoulderThrowerTexture() const;
    sf::Texture &getHowlerTexture() const;
    sf::Texture &getProwlerTexture() const;
    sf::Texture &getNecromancerTexture() const;
    sf::Texture &getWizardTexture() const;

    bool toBeRemoved; // Flag to indicate whether this HellPawn should be removed after capture
};

#endif // HELLPAWN_H
