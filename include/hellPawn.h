#ifndef HELLPAWN_H
#define HELLPAWN_H

#include "piece.h"
#include "textureManager.h"

class HellPawn : public Piece
{
public:
    HellPawn(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void infect(const Piece *capturingPiece, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board, TextureManager &textureManager);

    std::string getType() const override { return "HellPawn"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<HellPawn>(*this);
    }

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
    sf::Texture &getBeastDruidTexture() const;
    sf::Texture &getBeholderTexture() const;
    sf::Texture &getDeadLauncherTexture() const;
    sf::Texture &getPortalTexture() const;
    sf::Texture &getGhostKnightTexture() const;
    sf::Texture &getFamiliarTexture() const;
    sf::Texture &getQueenOfIllusionsTexture() const;
    sf::Texture &getQueenOfDestructionTexture() const;
    sf::Texture &getQueenOfDominationTexture() const;
    sf::Texture &getQueenOfBonesTexture() const;
    sf::Texture &getGhoulKingTexture() const;
    sf::Texture &getFrogKingTexture() const;
    sf::Texture &getHellKingTexture() const;
    sf::Texture &getWizardKingTexture() const;

    bool toBeRemoved; // Flag to indicate whether this HellPawn should be removed after capture
};

#endif // HELLPAWN_H
