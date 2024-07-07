#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "piece.h"
#include "square.h"
#include "necromancer.h"

class Game
{
public:
    void runChessGame(sf::RenderWindow &window);
};

#endif
