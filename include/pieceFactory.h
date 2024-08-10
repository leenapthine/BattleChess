#ifndef PIECE_FACTORY_H
#define PIECE_FACTORY_H

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include "piece.h"
#include "textureManager.h"

// Forward declarations for specific piece types
class Rook;
class Bishop;
class Knight;
class Pawn;
class Queen;
class King;
class NecroPawn;
class Necromancer;
class GhostKnight;
class DeadLauncher;
class HellPawn;
class Howler;
class Prowler;
class Beholder;
class YoungWiz;
class Wizard;
class Familiar;
class Portal;
class PawnHopper;
class BeastKnight;
class BeastDruid;
class BoulderThrower;
class QueenOfIllusions;
class QueenOfBones;
class QueenOfDomination;
class QueenOfDestruction;
class GhoulKing;
class WizardKing;
class FrogKing;
class HellKing;

// Define a factory function type
using PieceFactory = std::function<std::unique_ptr<Piece>(sf::Texture &, const sf::Vector2f &, Piece::Color)>;

// Declare the map of piece types to factory functions
extern std::unordered_map<std::string, PieceFactory> pieceFactoryMap;

std::unique_ptr<Piece> createPiece(const std::string &type, TextureManager &textureManager, const sf::Vector2f &position, Piece::Color color);

#endif // PIECE_FACTORY_H
