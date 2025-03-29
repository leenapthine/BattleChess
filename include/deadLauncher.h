#ifndef DEADLAUNCHER_H
#define DEADLAUNCHER_H

#include "piece.h"
#include <SFML/Graphics.hpp>

class DeadLauncher : public Piece
{
public:
    DeadLauncher(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightDeadCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces);

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    void highlightAdjacentPawns(std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board);

    bool isTargetWithinRange(const sf::Vector2f &target) const;

    std::string getType() const override { return "DeadLauncher"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<DeadLauncher>(*this);
    }

    bool pawnLoaded = false;
    const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
};

#endif // DEADLAUNCHER_H