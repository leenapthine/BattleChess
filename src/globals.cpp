// globals.cpp
// compilation of the global variables for our program

#include "globals.h"

const int BOARD_SIZE = 8;
const int TILE_SIZE = 80;
const int WINDOW_WIDTH = BOARD_SIZE * TILE_SIZE;
const int BUTTON_SIZE = 50;
const int BUTTON_SPACING = 10;
bool isPlayerWhite = true;

// iterator
int currentID = 0;

int getNextID()
{
    return currentID++;
}

sf::Texture whiteRookTexture;
sf::Texture blackRookTexture;
sf::Texture whitePawnTexture;
sf::Texture blackPawnTexture;
sf::Texture whiteKingTexture;
sf::Texture blackKingTexture;
sf::Texture whiteQueenTexture;
sf::Texture blackQueenTexture;
sf::Texture whiteBishopTexture;
sf::Texture blackBishopTexture;
sf::Texture whiteKnightTexture;
sf::Texture blackKnightTexture;
sf::Texture whiteWizardTexture;
sf::Texture blackWizardTexture;
sf::Texture whiteNecromancerTexture;
sf::Texture blackNecromancerTexture;
sf::Texture whiteNecroPawnTexture;
sf::Texture blackNecroPawnTexture;
sf::Texture whiteBeastKnightTexture;
sf::Texture blackBeastKnightTexture;
sf::Texture blackBoulderThrowerTexture;
sf::Texture whiteBoulderThrowerTexture;
sf::Texture whiteHowlerTexture;
sf::Texture blackHowlerTexture;
sf::Texture whiteProwlerTexture;
sf::Texture blackProwlerTexture;
sf::Texture whiteHellPawnTexture;
sf::Texture blackHellPawnTexture;
sf::Texture blackYoungWizTexture;
sf::Texture whiteYoungWizTexture;
sf::Texture blackPawnHopperTexture;
sf::Texture whitePawnHopperTexture;
sf::Texture blackBeastDruidTexture;
sf::Texture whiteBeastDruidTexture;
sf::Texture blackGhostKnightTexture;
sf::Texture whiteGhostKnightTexture;
sf::Texture blackFamiliarTexture;
sf::Texture whiteFamiliarTexture;
sf::Texture blackDeadLauncherTexture;
sf::Texture whiteDeadLauncherTexture;
sf::Texture blackPortalTexture;
sf::Texture whitePortalTexture;
sf::Texture blackBeholderTexture;
sf::Texture whiteBeholderTexture;
sf::Texture blackQueenOfIllusionsTexture;
sf::Texture whiteQueenOfIllusionsTexture;
sf::Texture blackQueenOfBonesTexture;
sf::Texture whiteQueenOfBonesTexture;
sf::Texture blackQueenOfDominationTexture;
sf::Texture whiteQueenOfDominationTexture;
sf::Texture blackQueenOfDestructionTexture;
sf::Texture whiteQueenOfDestructionTexture;
sf::Texture whiteWizardKingTexture;
sf::Texture blackWizardKingTexture;
sf::Texture whiteGhoulKingTexture;
sf::Texture blackGhoulKingTexture;
sf::Texture whiteFrogKingTexture;
sf::Texture blackFrogKingTexture;
sf::Texture whiteHellKingTexture;
sf::Texture blackHellKingTexture;