#ifndef PORTAL_H
#define PORTAL_H

#include "piece.h"
#include <SFML/Graphics.hpp>

class Portal : public Piece
{
public:
    Portal(sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void captureWithPortals(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces, std::vector<Portal *> &portals, std::vector<std::vector<Square>> &board);

    void teleport(sf::RenderWindow &window, std::vector<std::vector<Square>> &board, std::vector<std::unique_ptr<Piece>> &pieces, bool isWhiteTurn, std::vector<Portal *> &portals);

    void highlightValidUnloadPos(const sf::Vector2f &position, std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces);

    void highlightAdjacentPieces(std::vector<std::unique_ptr<Piece>> &pieces, std::vector<std::vector<Square>> &board, bool isWhiteTurn);

    // variables
    bool pieceLoaded = false;
    const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    Piece *pieceInPortal;

    std::string getType() const override { return "Portal"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<Portal>(*this);
    }
};

#endif // PORTAL_H