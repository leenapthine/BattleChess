#ifndef QUEENOFDOMINATION_H
#define QUEENOFDOMINATION_H

#include "piece.h"
#include "textureManager.h"
#include "square.h"
#include <vector>

class QueenOfDomination : public Piece
{
public:
    QueenOfDomination(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color), abilityUsed(false), highlightsActive(false) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    std::string getType() const override { return "QueenOfDomination"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<QueenOfDomination>(*this);
    }

    // New methods for this piece
    void highlightAdjacentFriendlyPieces(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const;
    void toggleAdjacentFriendlyHighlights(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const;
    void clearHighlights(std::vector<std::vector<Square>> &board) const;
    void applyDominationAbility(Piece *targetPiece, std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager) const;
    void returnOriginalSprite(std::vector<std::unique_ptr<Piece>> &pieces, TextureManager &textureManager);

    // Check if the ability has been used
    bool hasUsedAbility() const;

    // Tracks the piece that has been dominated
    mutable std::string originalType = "None";
    mutable Piece::Color originalColor = color;
    mutable sf::Texture *originalTexture = nullptr;
    mutable int originalID;

private:
    mutable bool abilityUsed;      // Track whether the ability has been used
    mutable bool highlightsActive; // Track if highlights are active
};

#endif // QUEENOFDOMINATION_H