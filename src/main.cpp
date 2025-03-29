#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "menu.h"
#include "game.h"
#include "network.h"
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

    // network operations
    std::unique_ptr<sf::TcpSocket> socket = initialConnect();

    std::cout << "Finished initialConnect" << std::endl;

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
            game.runChessGame(window, socket);
        }
    }

    return 0;
}
