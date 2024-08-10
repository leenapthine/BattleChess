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
    {"Rook", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Rook>(texture, pos, color);
     }},
    {"Bishop", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Bishop>(texture, pos, color);
     }},
    {"Knight", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Knight>(texture, pos, color);
     }},
    {"Pawn", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Pawn>(texture, pos, color);
     }},
    {"Queen", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Queen>(texture, pos, color);
     }},
    {"King", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<King>(texture, pos, color);
     }},
    {"NecroPawn", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<NecroPawn>(texture, pos, color);
     }},
    {"Necromancer", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Necromancer>(texture, pos, color);
     }},
    {"GhostKnight", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<GhostKnight>(texture, pos, color);
     }},
    {"DeadLauncher", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<DeadLauncher>(texture, pos, color);
     }},
    {"HellPawn", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<HellPawn>(texture, pos, color);
     }},
    {"Howler", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Howler>(texture, pos, color);
     }},
    {"Prowler", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Prowler>(texture, pos, color);
     }},
    {"Beholder", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Beholder>(texture, pos, color);
     }},
    {"YoungWiz", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<YoungWiz>(texture, pos, color);
     }},
    {"Wizard", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Wizard>(texture, pos, color);
     }},
    {"Familiar", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Familiar>(texture, pos, color);
     }},
    {"Portal", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<Portal>(texture, pos, color);
     }},
    {"PawnHopper", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<PawnHopper>(texture, pos, color);
     }},
    {"BeastKnight", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<BeastKnight>(texture, pos, color);
     }},
    {"BeastDruid", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<BeastDruid>(texture, pos, color);
     }},
    {"BoulderThrower", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<BoulderThrower>(texture, pos, color);
     }},
    {"QueenOfIllusions", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfIllusions>(texture, pos, color);
     }},
    {"QueenOfBones", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfBones>(texture, pos, color);
     }},
    {"QueenOfDomination", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfDomination>(texture, pos, color);
     }},
    {"QueenOfDestruction", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<QueenOfDestruction>(texture, pos, color);
     }},
    {"GhoulKing", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<GhoulKing>(texture, pos, color);
     }},
    {"WizardKing", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<WizardKing>(texture, pos, color);
     }},
    {"FrogKing", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<FrogKing>(texture, pos, color);
     }},
    {"HellKing", [](sf::Texture &texture, const sf::Vector2f &pos, Piece::Color color)
     {
         return std::make_unique<HellKing>(texture, pos, color);
     }}};

std::unique_ptr<Piece> createPiece(const std::string &type, TextureManager &textureManager, const sf::Vector2f &position, Piece::Color color)
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
        auto piece = it->second(*texture, position, color);

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
