#ifndef HOWLER_H
#define HOWLER_H

#include "bishop.h"
#include <unordered_set>
#include <functional>

class Howler : public Piece
{
public:
    Howler(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color),
          bishopAbilities(false),
          rookAbilities(false),
          knightAbilities(false),
          pawnAbilities(false),
          queenAbilities(false),
          kingAbilities(false)
    {
        gainInitialAbilities();
    }

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;
    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::string getType() const override { return "Howler"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Howler>(*this);
    }

    bool bishopAbilities;
    bool rookAbilities;
    bool knightAbilities;
    bool pawnAbilities;
    bool queenAbilities;
    bool kingAbilities;

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

    void gainAbilitiesFromCapturedPiece(const Piece &capturedPiece);
};

#endif // HOWLER_H