#ifndef WIZARDKING_H
#define WIZARDKING_H

#include "piece.h"

class WizardKing : public Piece
{
public:
    WizardKing(int id, sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : Piece(id, texture, initialPosition, color) {}

    bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const override;

    void highlightCaptureZones(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const;

    void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces) override;

    std::vector<sf::Vector2f> getCapturePositions() const;

    std::string getType() const override { return "WizardKing"; }

    std::unique_ptr<Piece> clone() const override
    {
        return std::make_unique<WizardKing>(*this);
    }
};

#endif // WIZARDKING_H