#ifndef UTILITY_H
#define UTILITY_H

#include "piece.h" // Include piece.h to access the Color enum
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "button.h"
#include "textureManager.h"
#include <vector>
#include <memory>

// Type for the piece creation function
using PieceFactory = std::function<std::unique_ptr<Piece>(sf::Texture &, const sf::Vector2f &, Piece::Color)>;

// Function to create a piece using the type name
std::unique_ptr<Piece> createPiece(const std::string &type, sf::Texture &texture, const sf::Vector2f &position, Piece::Color color);

bool isPathClear(const sf::Vector2f &start, const sf::Vector2f &end, const std::vector<std::unique_ptr<Piece>> &pieces);

void loadTextures(TextureManager &textureManager);

void createPieces(TextureManager &textureManager, std::vector<std::unique_ptr<Piece>> &pieces);

Piece *getPieceAtPosition(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces);

bool isOpponentPiece(const sf::Vector2f &position, const std::vector<std::unique_ptr<Piece>> &pieces, Piece::Color currentPlayerColor);

void drawChessboard(sf::RenderWindow &window, std::vector<std::vector<Square>> &board);

void pushPawn(std::vector<std::unique_ptr<Piece>> &pieces, sf::Texture &pawnTexture, const sf::Vector2f &position, Piece::Color pieceColor);

void pushNecroPawn(std::vector<std::unique_ptr<Piece>> &pieces, sf::Texture &necroPawnTexture, const sf::Vector2f &position, Piece::Color pieceColor);

void highlightButtonOnHover(sf::RenderWindow &window, Button &button, const sf::Color &normalColor, const sf::Color &hoverColor);

sf::Vector2i getUserInput();

sf::Vector2f calculateNextPosition(const sf::Vector2f &currentPosition, const sf::Vector2i &movement);

#endif
