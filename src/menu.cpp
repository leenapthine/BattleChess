#include "menu.h"
#include "button.h"
#include "utility.h"
#include "globals.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void necroArmyMenu(sf::RenderWindow &window, sf::Font &font, bool &inNecroMenu)
{
    int gemstones = 20;

    // Colors for button states
    sf::Color normalColor(0, 165, 0);
    sf::Color hoverColor(144, 238, 144);
    sf::Color selectedColor(255, 255, 0); // Yellow for selected buttons

    sf::Text assembleArmy("Assemble Your Army", font, 40);
    assembleArmy.setFillColor(sf::Color::White);
    assembleArmy.setPosition((WINDOW_WIDTH - assembleArmy.getGlobalBounds().width) / 2, 80);

    // Text to display remaining gemstones
    sf::Text gemstoneText("Gemstones: " + std::to_string(gemstones), font, 30);
    gemstoneText.setFillColor(sf::Color::White);
    gemstoneText.setPosition(10, 10);

    // Create buttons for chess pieces
    std::vector<Button> buttons;
    std::vector<int> costs = {-4, -1, -3, -1, -3, -1, -2, -1, -5, -1, -3, -1, -3, -1, -4, -1};
    std::vector<int> costsText = {-4, -3, -3, -2, -5, -3, -3, -4, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<std::string> pieceNames = {"R1", "K1", "B1", "King", "Queen", "B2", "K2", "R2",
                                           "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8"};

    std::vector<std::string> upgradeNames = {"Rook", "NecroPawn", "Knight", "NecroPawn", "Necromancer", "NecroPawn", "King", "NecroPawn",
                                             "Queen", "NecroPawn", "Necromancer", "NecroPawn", "Knight", "NecroPawn", "Rook", "NecroPawn"};

    int startX = (WINDOW_WIDTH - (BOARD_SIZE * (BUTTON_SIZE + BUTTON_SPACING) - BUTTON_SPACING)) / 2;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 200), std::to_string(costsText[i]), font));
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 300), std::to_string(costsText[i + BOARD_SIZE]), font));
    }

    // Create text for piece names above each button
    std::vector<sf::Text> pieceTexts;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Text pieceText(pieceNames[i], font, 14);
        pieceText.setFillColor(sf::Color::White);
        pieceText.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 170);
        pieceTexts.push_back(pieceText);

        sf::Text pieceText2(pieceNames[i + BOARD_SIZE], font, 14);
        pieceText2.setFillColor(sf::Color::White);
        pieceText2.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 270);
        pieceTexts.push_back(pieceText2);
    }

    Button startButton(sf::Vector2f(200, 50), sf::Vector2f((WINDOW_WIDTH - 200) / 2, 500), "Start Game", font);

    sf::Text hoverText("", font, 20);
    hoverText.setFillColor(sf::Color::White);
    hoverText.setStyle(sf::Text::Bold);

    std::vector<bool> selected(buttons.size(), false); // Track selected state of buttons

    while (inNecroMenu && window.isOpen())
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

                for (int i = 0; i < buttons.size(); ++i)
                {
                    if (buttons[i].isClicked(mousePos))
                    {
                        if (!selected[i] && gemstones + costs[i] >= 0) // Select button
                        {
                            gemstones += costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(selectedColor); // Change color to selected
                            selected[i] = true;                                      // Mark as selected
                            // Here, you would implement the logic to upgrade the piece
                            // For example, upgradePiece(pieces[i]);
                        }
                        else if (selected[i]) // Unselect button
                        {
                            gemstones -= costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(normalColor); // Change color to normal
                            selected[i] = false;                                   // Mark as unselected
                        }
                    }
                }

                if (startButton.isClicked(mousePos))
                {
                    inNecroMenu = false;
                    // Start the game with the upgraded pieces
                    // startGame();
                }
            }
        }

        // Highlight buttons on hover and show hover text
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool hover = false;

        for (int i = 0; i < buttons.size(); ++i)
        {
            if (!selected[i]) // Only change color on hover if not selected
            {
                highlightButtonOnHover(window, buttons[i], normalColor, hoverColor);
                if (buttons[i].isHovered(mousePos))
                {
                    hover = true;
                    hoverText.setString(upgradeNames[i]);
                    hoverText.setPosition(mousePos.x + 15, mousePos.y + 15);
                }
            }
        }

        highlightButtonOnHover(window, startButton, normalColor, hoverColor);

        window.clear();
        window.draw(assembleArmy);
        window.draw(gemstoneText);
        for (auto &pieceText : pieceTexts)
        {
            window.draw(pieceText);
        }
        for (auto &button : buttons)
        {
            button.draw(window);
        }
        startButton.draw(window);

        if (hover)
        {
            window.draw(hoverText);
        }

        window.display();
    }
}

void wizardArmyMenu(sf::RenderWindow &window, sf::Font &font, bool &inWizMenu)
{
    int gemstones = 20;

    // Colors for button states
    sf::Color normalColor(0, 165, 0);
    sf::Color hoverColor(144, 238, 144);
    sf::Color selectedColor(255, 255, 0); // Yellow for selected buttons

    sf::Text assembleArmy("Assemble Your Army", font, 40);
    assembleArmy.setFillColor(sf::Color::White);
    assembleArmy.setPosition((WINDOW_WIDTH - assembleArmy.getGlobalBounds().width) / 2, 80);

    // Text to display remaining gemstones
    sf::Text gemstoneText("Gemstones: " + std::to_string(gemstones), font, 30);
    gemstoneText.setFillColor(sf::Color::White);
    gemstoneText.setPosition(10, 10);

    // Create buttons for chess pieces
    std::vector<Button> buttons;
    std::vector<int> costs = {-4, -1, -3, -1, -3, -1, -2, -1, -5, -1, -3, -1, -3, -1, -4, -1};
    std::vector<int> costsText = {-4, -3, -3, -2, -5, -3, -3, -4, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<std::string> pieceNames = {"R1", "K1", "B1", "King", "Queen", "B2", "K2", "R2",
                                           "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8"};

    std::vector<std::string> upgradeNames = {"Rook", "YoungWiz", "Knight", "YoungWiz", "Wizard", "YoungWiz", "King", "YoungWiz",
                                             "Queen", "YoungWiz", "Wizard", "YoungWiz", "Knight", "YoungWiz", "Rook", "YoungWiz"};

    int startX = (WINDOW_WIDTH - (BOARD_SIZE * (BUTTON_SIZE + BUTTON_SPACING) - BUTTON_SPACING)) / 2;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 200), std::to_string(costsText[i]), font));
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 300), std::to_string(costsText[i + BOARD_SIZE]), font));
    }

    // Create text for piece names above each button
    std::vector<sf::Text> pieceTexts;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Text pieceText(pieceNames[i], font, 14);
        pieceText.setFillColor(sf::Color::White);
        pieceText.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 170);
        pieceTexts.push_back(pieceText);

        sf::Text pieceText2(pieceNames[i + BOARD_SIZE], font, 14);
        pieceText2.setFillColor(sf::Color::White);
        pieceText2.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 270);
        pieceTexts.push_back(pieceText2);
    }

    Button startButton(sf::Vector2f(200, 50), sf::Vector2f((WINDOW_WIDTH - 200) / 2, 500), "Start Game", font);

    sf::Text hoverText("", font, 20);
    hoverText.setFillColor(sf::Color::White);
    hoverText.setStyle(sf::Text::Bold);

    std::vector<bool> selected(buttons.size(), false); // Track selected state of buttons

    while (inWizMenu && window.isOpen())
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

                for (int i = 0; i < buttons.size(); ++i)
                {
                    if (buttons[i].isClicked(mousePos))
                    {
                        if (!selected[i] && gemstones + costs[i] >= 0) // Select button
                        {
                            gemstones += costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(selectedColor); // Change color to selected
                            selected[i] = true;                                      // Mark as selected
                            // Here, you would implement the logic to upgrade the piece
                            // For example, upgradePiece(pieces[i]);
                        }
                        else if (selected[i]) // Unselect button
                        {
                            gemstones -= costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(normalColor); // Change color to normal
                            selected[i] = false;                                   // Mark as unselected
                        }
                    }
                }

                if (startButton.isClicked(mousePos))
                {
                    inWizMenu = false;
                    // Start the game with the upgraded pieces
                    // startGame();
                }
            }
        }

        // Highlight buttons on hover and show hover text
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool hover = false;

        for (int i = 0; i < buttons.size(); ++i)
        {
            if (!selected[i]) // Only change color on hover if not selected
            {
                highlightButtonOnHover(window, buttons[i], normalColor, hoverColor);
                if (buttons[i].isHovered(mousePos))
                {
                    hover = true;
                    hoverText.setString(upgradeNames[i]);
                    hoverText.setPosition(mousePos.x + 15, mousePos.y + 15);
                }
            }
        }

        highlightButtonOnHover(window, startButton, normalColor, hoverColor);

        window.clear();
        window.draw(assembleArmy);
        window.draw(gemstoneText);
        for (auto &pieceText : pieceTexts)
        {
            window.draw(pieceText);
        }
        for (auto &button : buttons)
        {
            button.draw(window);
        }
        startButton.draw(window);

        if (hover)
        {
            window.draw(hoverText);
        }

        window.display();
    }
}

void beastArmyMenu(sf::RenderWindow &window, sf::Font &font, bool &inBeastMenu)
{
    int gemstones = 20;

    // Colors for button states
    sf::Color normalColor(0, 165, 0);
    sf::Color hoverColor(144, 238, 144);
    sf::Color selectedColor(255, 255, 0); // Yellow for selected buttons

    sf::Text assembleArmy("Assemble Your Army", font, 40);
    assembleArmy.setFillColor(sf::Color::White);
    assembleArmy.setPosition((WINDOW_WIDTH - assembleArmy.getGlobalBounds().width) / 2, 80);

    // Text to display remaining gemstones
    sf::Text gemstoneText("Gemstones: " + std::to_string(gemstones), font, 30);
    gemstoneText.setFillColor(sf::Color::White);
    gemstoneText.setPosition(10, 10);

    // Create buttons for chess pieces
    std::vector<Button> buttons;
    std::vector<int> costs = {-4, -1, -3, -1, -3, -1, -2, -1, -5, -1, -3, -1, -3, -1, -4, -1};
    std::vector<int> costsText = {-4, -3, -3, -2, -5, -3, -3, -4, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<std::string> pieceNames = {"R1", "K1", "B1", "King", "Queen", "B2", "K2", "R2",
                                           "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8"};

    std::vector<std::string> upgradeNames = {"Boulder Thrower", "Pawn", "BeastKnight", "Pawn", "Bishop", "Pawn", "King", "Pawn",
                                             "Queen", "Pawn", "Bishop", "Pawn", "BeastKnight", "Pawn", "Boulder Thrower", "Pawn"};

    int startX = (WINDOW_WIDTH - (BOARD_SIZE * (BUTTON_SIZE + BUTTON_SPACING) - BUTTON_SPACING)) / 2;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 200), std::to_string(costsText[i]), font));
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 300), std::to_string(costsText[i + BOARD_SIZE]), font));
    }

    // Create text for piece names above each button
    std::vector<sf::Text> pieceTexts;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Text pieceText(pieceNames[i], font, 14);
        pieceText.setFillColor(sf::Color::White);
        pieceText.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 170);
        pieceTexts.push_back(pieceText);

        sf::Text pieceText2(pieceNames[i + BOARD_SIZE], font, 14);
        pieceText2.setFillColor(sf::Color::White);
        pieceText2.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 270);
        pieceTexts.push_back(pieceText2);
    }

    Button startButton(sf::Vector2f(200, 50), sf::Vector2f((WINDOW_WIDTH - 200) / 2, 500), "Start Game", font);

    sf::Text hoverText("", font, 20);
    hoverText.setFillColor(sf::Color::White);
    hoverText.setStyle(sf::Text::Bold);

    std::vector<bool> selected(buttons.size(), false); // Track selected state of buttons

    while (inBeastMenu && window.isOpen())
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

                for (int i = 0; i < buttons.size(); ++i)
                {
                    if (buttons[i].isClicked(mousePos))
                    {
                        if (!selected[i] && gemstones + costs[i] >= 0) // Select button
                        {
                            gemstones += costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(selectedColor); // Change color to selected
                            selected[i] = true;                                      // Mark as selected
                            // Here, you would implement the logic to upgrade the piece
                            // For example, upgradePiece(pieces[i]);
                        }
                        else if (selected[i]) // Unselect button
                        {
                            gemstones -= costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(normalColor); // Change color to normal
                            selected[i] = false;                                   // Mark as unselected
                        }
                    }
                }

                if (startButton.isClicked(mousePos))
                {
                    inBeastMenu = false;
                    // Start the game with the upgraded pieces
                    // startGame();
                }
            }
        }

        // Highlight buttons on hover and show hover text
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool hover = false;

        for (int i = 0; i < buttons.size(); ++i)
        {
            if (!selected[i]) // Only change color on hover if not selected
            {
                highlightButtonOnHover(window, buttons[i], normalColor, hoverColor);
                if (buttons[i].isHovered(mousePos))
                {
                    hover = true;
                    hoverText.setString(upgradeNames[i]);
                    hoverText.setPosition(mousePos.x + 15, mousePos.y + 15);
                }
            }
        }

        highlightButtonOnHover(window, startButton, normalColor, hoverColor);

        window.clear();
        window.draw(assembleArmy);
        window.draw(gemstoneText);
        for (auto &pieceText : pieceTexts)
        {
            window.draw(pieceText);
        }
        for (auto &button : buttons)
        {
            button.draw(window);
        }
        startButton.draw(window);

        if (hover)
        {
            window.draw(hoverText);
        }

        window.display();
    }
}

void hellArmyMenu(sf::RenderWindow &window, sf::Font &font, bool &inHellMenu)
{
    int gemstones = 20;

    // Colors for button states
    sf::Color normalColor(0, 165, 0);
    sf::Color hoverColor(144, 238, 144);
    sf::Color selectedColor(255, 255, 0); // Yellow for selected buttons

    sf::Text assembleArmy("Assemble Your Army", font, 40);
    assembleArmy.setFillColor(sf::Color::White);
    assembleArmy.setPosition((WINDOW_WIDTH - assembleArmy.getGlobalBounds().width) / 2, 80);

    // Text to display remaining gemstones
    sf::Text gemstoneText("Gemstones: " + std::to_string(gemstones), font, 30);
    gemstoneText.setFillColor(sf::Color::White);
    gemstoneText.setPosition(10, 10);

    // Create buttons for chess pieces
    std::vector<Button> buttons;
    std::vector<int> costs = {-4, -1, -3, -1, -3, -1, -2, -1, -5, -1, -3, -1, -3, -1, -4, -1};
    std::vector<int> costsText = {-4, -3, -3, -2, -5, -3, -3, -4, -1, -1, -1, -1, -1, -1, -1, -1};
    std::vector<std::string> pieceNames = {"R1", "K1", "B1", "King", "Queen", "B2", "K2", "R2",
                                           "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8"};

    std::vector<std::string> upgradeNames = {"Rook", "HellPawn", "Prowler", "HellPawn", "Howler", "HellPawn", "King", "HellPawn",
                                             "Queen", "HellPawn", "Howler", "HellPawn", "Prowler", "HellPawn", "Rook", "HellPawn"};

    int startX = (WINDOW_WIDTH - (BOARD_SIZE * (BUTTON_SIZE + BUTTON_SPACING) - BUTTON_SPACING)) / 2;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 200), std::to_string(costsText[i]), font));
        buttons.push_back(Button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE), sf::Vector2f(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 300), std::to_string(costsText[i + BOARD_SIZE]), font));
    }

    // Create text for piece names above each button
    std::vector<sf::Text> pieceTexts;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        sf::Text pieceText(pieceNames[i], font, 14);
        pieceText.setFillColor(sf::Color::White);
        pieceText.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 170);
        pieceTexts.push_back(pieceText);

        sf::Text pieceText2(pieceNames[i + BOARD_SIZE], font, 14);
        pieceText2.setFillColor(sf::Color::White);
        pieceText2.setPosition(startX + i * (BUTTON_SIZE + BUTTON_SPACING), 270);
        pieceTexts.push_back(pieceText2);
    }

    Button startButton(sf::Vector2f(200, 50), sf::Vector2f((WINDOW_WIDTH - 200) / 2, 500), "Start Game", font);

    sf::Text hoverText("", font, 20);
    hoverText.setFillColor(sf::Color::White);
    hoverText.setStyle(sf::Text::Bold);

    std::vector<bool> selected(buttons.size(), false); // Track selected state of buttons

    while (inHellMenu && window.isOpen())
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

                for (int i = 0; i < buttons.size(); ++i)
                {
                    if (buttons[i].isClicked(mousePos))
                    {
                        if (!selected[i] && gemstones + costs[i] >= 0) // Select button
                        {
                            gemstones += costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(selectedColor); // Change color to selected
                            selected[i] = true;                                      // Mark as selected
                            // Here, you would implement the logic to upgrade the piece
                            // For example, upgradePiece(pieces[i]);
                        }
                        else if (selected[i]) // Unselect button
                        {
                            gemstones -= costs[i]; // Update gemstone count
                            gemstoneText.setString("Gemstones: " + std::to_string(gemstones));
                            buttons[i].getButtonShape().setFillColor(normalColor); // Change color to normal
                            selected[i] = false;                                   // Mark as unselected
                        }
                    }
                }

                if (startButton.isClicked(mousePos))
                {
                    inHellMenu = false;
                    // Start the game with the upgraded pieces
                    // startGame();
                }
            }
        }

        // Highlight buttons on hover and show hover text
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool hover = false;

        for (int i = 0; i < buttons.size(); ++i)
        {
            if (!selected[i]) // Only change color on hover if not selected
            {
                highlightButtonOnHover(window, buttons[i], normalColor, hoverColor);
                if (buttons[i].isHovered(mousePos))
                {
                    hover = true;
                    hoverText.setString(upgradeNames[i]);
                    hoverText.setPosition(mousePos.x + 15, mousePos.y + 15);
                }
            }
        }

        highlightButtonOnHover(window, startButton, normalColor, hoverColor);

        window.clear();
        window.draw(assembleArmy);
        window.draw(gemstoneText);
        for (auto &pieceText : pieceTexts)
        {
            window.draw(pieceText);
        }
        for (auto &button : buttons)
        {
            button.draw(window);
        }
        startButton.draw(window);

        if (hover)
        {
            window.draw(hoverText);
        }

        window.display();
    }
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
                    bool inNecroMenu = true;
                    necroArmyMenu(window, font, inNecroMenu);
                }
                else if (race2Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                    bool inWizMenu = true;
                    wizardArmyMenu(window, font, inWizMenu);
                }
                else if (race3Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                    bool inBeastMenu = true;
                    beastArmyMenu(window, font, inBeastMenu);
                }
                else if (race4Button.isClicked(mousePos))
                {
                    inRaceMenu = false;
                    bool inHellMenu = true;
                    hellArmyMenu(window, font, inHellMenu);
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
