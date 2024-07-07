#include <SFML/Graphics.hpp>
#include "menu.h"
#include "game.h"
#include <iostream>
#include <vector>

int main()
{ // Make a menu window
    sf::RenderWindow window(sf::VideoMode(640, 640), "Battle Chess");

    sf::Font font;
    if (!font.loadFromFile("../resources/JmhcthulhumbusarcadeugRegular-JleB.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    bool inMainMenu = true;

    while (window.isOpen())
    {
        if (inMainMenu)
        {
            showMainMenu(window, font, inMainMenu);
        }
        else
        {
            Game game;
            game.runChessGame(window);
        }
    }

    return 0;
}
