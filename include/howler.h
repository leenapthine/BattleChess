#ifndef HOWLER_H
#define HOWLER_H

#include "bishop.h"
#include <unordered_set>
#include <functional>

class Howler : public Piece
{
public:
    Howler(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color),
          boulderThrowerAbilities(false),
          wizardAbilities(false),
          bishopAbilities(false),
          rookAbilities(false),
          knightAbilities(false),
          pawnAbilities(false),
          queenAbilities(false),
          kingAbilities(false),
          beastKnightAbilities(false),
          necromancerAbilities(false),
          necroPawnAbilities(false)
    {
        gainInitialAbilities();
    }

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;
    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "Howler"; }

    bool boulderThrowerAbilities;
    bool wizardAbilities;
    bool bishopAbilities;
    bool rookAbilities;
    bool knightAbilities;
    bool pawnAbilities;
    bool queenAbilities;
    bool kingAbilities;
    bool beastKnightAbilities;
    bool necromancerAbilities;
    bool necroPawnAbilities;

private:
    std::vector<std::function<bool(const sf::Vector2f &, const std::vector<std::unique_ptr<Piece>> &)>> movementAbilities;
    std::vector<std::function<void(std::vector<std::vector<Square>> &, const std::vector<std::unique_ptr<Piece>> &)>> highlightAbilities;
    std::vector<std::function<void(const sf::Vector2f &, std::vector<std::unique_ptr<Piece>> &)>> captureAbilities;

    void gainInitialAbilities();
    void gainBishopAbilities();
    void gainRookAbilities();
    void gainKnightAbilities();
    void gainPawnAbilities();
    void gainQueenAbilities();
    void gainKingAbilities();
    void gainBeastKnightAbilities();
    void gainBoulderThrowerAbilities();
    void gainNecromancerAbilities();
    void gainNecroPawnAbilities();
    void gainWizardAbilities();
    void gainAbilitiesFromCapturedPiece(const Piece &capturedPiece);
};

#endif // HOWLER_H