#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

void chooseRaceMenu(sf::RenderWindow &window, sf::Font &font, bool &inRaceMenu);
void showMainMenu(sf::RenderWindow &window, sf::Font &font, bool &inMainMenu);
void necroArmyMenu(sf::RenderWindow &window, sf::Font &font, bool &inBuildMenu);

#endif
