#ifndef UTILITY_H
#define UTILITY_H

#include "piece.h" // Include piece.h to access the Color enum
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "button.h"

bool isPathClear(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces);

void loadTextures(std::vector<std::unique_ptr<Piece>> &pieces);

Piece *getPieceAtPosition(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces);

bool isOpponentPiece(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces, Piece::Color currentPlayerColor);

void drawChessboard(sf::RenderWindow &window, std::vector<std::vector<Square>> &board);

void pushPawn(std::vector<std::unique_ptr<Piece>> &pieces, sf::Texture &pawnTexture, const sf::Vector2f &position, Piece::Color pieceColor);

void highlightButtonOnHover(sf::RenderWindow &window, Button &button, const sf::Color &normalColor, const sf::Color &hoverColor);

sf::Vector2i getUserInput();

sf::Vector2f calculateNextPosition(const sf::Vector2f &currentPosition, const sf::Vector2i &movement);

#endif
