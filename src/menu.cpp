#include "menu.h"
#include "button.h"
#include "utility.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void necroArmyMenu(sf::RenderWindow &window, sf::Font &font, bool &inNecroMenu) 
{
    
}


void chooseRaceMenu(sf::RenderWindow &window, sf::Font &font, bool &inRaceMenu)
{
    // Colors for button states
    sf::Color normalColor(0, 165, 0);
    sf::Color hoverColor(144, 238, 144);

    sf::Text selectRace("Select Race", font, 40);
    selectRace.setFillColor(sf::Color::White);
    selectRace.setPosition(200, 100);

    sf::Text race1("The Necromancer race: The dead do your bidding", font, 12);
    race1.setFillColor(sf::Color::White);
    race1.setPosition(170, 182);

    Button race1Button(sf::Vector2f(200, 50), sf::Vector2f(220, 200), "Necromancers", font);

    sf::Text race2("The Wizarding race: Cast powerful spells", font, 12);
    race2.setFillColor(sf::Color::White);
    race2.setPosition(195, 262);

    Button race2Button(sf::Vector2f(200, 50), sf::Vector2f(220, 280), "Wizards", font);

    sf::Text race3("The Beast Master race: Ride monsters into battle", font, 12);
    race3.setFillColor(sf::Color::White);
    race3.setPosition(170, 342);

    Button race3Button(sf::Vector2f(200, 50), sf::Vector2f(220, 360), "Beast Masters", font);

    sf::Text race4("The Hellspwan race: Feed on the souls of opponents", font, 12);
    race4.setFillColor(sf::Color::White);
    race4.setPosition(165, 422);

    Button race4Button(sf::Vector2f(200, 50), sf::Vector2f(220, 440), "Hellspawn", font);

    while (inRaceMenu && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (race1Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                }
                else if (race2Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                }
                else if (race3Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                }
                else if (race4Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                }
            }
        }

        // Use the utility function to highlight the button on hover
        highlightButtonOnHover(window, race1Button, normalColor, hoverColor);
        highlightButtonOnHover(window, race2Button, normalColor, hoverColor);
        highlightButtonOnHover(window, race3Button, normalColor, hoverColor);
        highlightButtonOnHover(window, race4Button, normalColor, hoverColor);

        window.clear();
        window.draw(selectRace);
        window.draw(race1);
        window.draw(race2);
        window.draw(race3);
        window.draw(race4);
        race1Button.draw(window);
        race2Button.draw(window);
        race3Button.draw(window);
        race4Button.draw(window);
        window.display();
    }
}

void showMainMenu(sf::RenderWindow &window, sf::Font &font, bool &inMainMenu)
{
    sf::Text title("Battle Chess", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(158, 50);

    // Colors for button states
    sf::Color normalColor(0, 165, 0);
    sf::Color hoverColor(144, 238, 144);

    // set a background
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(640, 640));
    sf::Texture MainTexture;
    MainTexture.loadFromFile("../resources/BustedMenu.png");
    background.setTexture(&MainTexture);

    Button playButton(sf::Vector2f(200, 50), sf::Vector2f(220, 200), "Pick a Race", font);
    Button optionsButton(sf::Vector2f(200, 50), sf::Vector2f(220, 300), "Options", font);
    Button quitButton(sf::Vector2f(200, 50), sf::Vector2f(220, 400), "Quit", font);

    while (inMainMenu && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (playButton.isClicked(mousePos))
                {
                    inMainMenu = false;
                    bool inRaceMenu = true;
                    chooseRaceMenu(window, font, inRaceMenu);
                }
                else if (optionsButton.isClicked(mousePos))
                {
                    // Handle options here
                    std::cout << "Options selected" << std::endl;
                }
                else if (quitButton.isClicked(mousePos))
                {
                    window.close();
                }
            }
        }

        // Use the utility function to highlight the button on hover
        highlightButtonOnHover(window, playButton, normalColor, hoverColor);
        highlightButtonOnHover(window, optionsButton, normalColor, hoverColor);
        highlightButtonOnHover(window, quitButton, normalColor, hoverColor);

        window.clear();
        window.draw(background);
        window.draw(title);
        playButton.draw(window);
        optionsButton.draw(window);
        quitButton.draw(window);
        window.display();
    }
}
