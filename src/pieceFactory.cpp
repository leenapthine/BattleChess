#include "pieceFactory.h"
#include "textureManager.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "king.h"
#include "necroPawn.h"
#include "necromancer.h"
#include "ghostKnight.h"
#include "deadLauncher.h"
#include "hellPawn.h"
#include "howler.h"
#include "prowler.h"
#include "beholder.h"
#include "youngWiz.h"
#include "wizard.h"
#include "familiar.h"
#include "portal.h"
#include "pawnHopper.h"
#include "beastKnight.h"
#include "beastDruid.h"
#include "boulderThrower.h"
#include "queenOfIllusions.h"
#include "queenOfBones.h"
#include "queenOfDomination.h"
#include "queenOfDestruction.h"
#include "ghoulKing.h"
#include "frogKing.h"
#include "wizardKing.h"
#include "hellKing.h"

// Define the map of piece types to factory functions
std::unordered_map<std::string, PieceFactory> pieceFactoryMap = {
    {"Rook", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Rook>(id, texture, pos, color);
     }},
    {"Bishop", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Bishop>(id, texture, pos, color);
     }},
    {"Knight", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Knight>(id, texture, pos, color);
     }},
    {"Pawn", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Pawn>(id, texture, pos, color);
     }},
    {"Queen", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Queen>(id, texture, pos, color);
     }},
    {"King", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<King>(id, texture, pos, color);
     }},
    {"NecroPawn", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<NecroPawn>(id, texture, pos, color);
     }},
    {"Necromancer", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Necromancer>(id, texture, pos, color);
     }},
    {"GhostKnight", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<GhostKnight>(id, texture, pos, color);
     }},
    {"DeadLauncher", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<DeadLauncher>(id, texture, pos, color);
     }},
    {"HellPawn", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<HellPawn>(id, texture, pos, color);
     }},
    {"Howler", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Howler>(id, texture, pos, color);
     }},
    {"Prowler", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Prowler>(id, texture, pos, color);
     }},
    {"Beholder", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Beholder>(id, texture, pos, color);
     }},
    {"YoungWiz", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<YoungWiz>(id, texture, pos, color);
     }},
    {"Wizard", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Wizard>(id, texture, pos, color);
     }},
    {"Familiar", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Familiar>(id, texture, pos, color);
     }},
    {"Portal", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Portal>(id, texture, pos, color);
     }},
    {"PawnHopper", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<PawnHopper>(id, texture, pos, color);
     }},
    {"BeastKnight", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<BeastKnight>(id, texture, pos, color);
     }},
    {"BeastDruid", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<BeastDruid>(id, texture, pos, color);
     }},
    {"BoulderThrower", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<BoulderThrower>(id, texture, pos, color);
     }},
    {"QueenOfIllusions", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfIllusions>(id, texture, pos, color);
     }},
    {"QueenOfBones", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfBones>(id, texture, pos, color);
     }},
    {"QueenOfDomination", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfDomination>(id, texture, pos, color);
     }},
    {"QueenOfDestruction", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfDestruction>(id, texture, pos, color);
     }},
    {"GhoulKing", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<GhoulKing>(id, texture, pos, color);
     }},
    {"WizardKing", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<WizardKing>(id, texture, pos, color);
     }},
    {"FrogKing", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<FrogKing>(id, texture, pos, color);
     }},
    {"HellKing", [](int id, sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<HellKing>(id, texture, pos, color);
     }}};

std::unique_ptr<Piece> createPiece(int id, const std::string &type, TextureManager &textureManager, const sf::Vector2f &position, Piece::Color color)
{
    std::string textureName = (color == Piece::Color::White ? "White" : "Black") + type;
    sf::Texture *texture = textureManager.getTexture(textureName);

    if (!texture)
    {
        std::cerr << "Failed to load texture for: " << textureName << std::endl;
        return nullptr;
    }

    auto it = pieceFactoryMap.find(type);

    // Check if the factory function exists in the map
    if (it != pieceFactoryMap.end())
    {
        // Create the piece using the factory function
        auto piece = it->second(id, *texture, position, color);

        if (!piece)
        {
            std::cerr << "Failed to create piece for type: " << type << std::endl;
        }
        else
        {
            std::cout << "Queen Of Domination returns to being a piece of type: " << type << std::endl;
        }

        // Scale the size of sprite texture
        float scaleFactor = TILE_SIZE / static_cast<float>(std::max(textureManager.getTexture("WhiteRook")->getSize().x, textureManager.getTexture("WhiteRook")->getSize().y));
        piece->getSprite().setScale(scaleFactor, scaleFactor);

        return piece;
    }
    else
    {
        std::cerr << "No factory found for type: " << type << std::endl;
    }

    return nullptr; // Or handle the error appropriately
}
