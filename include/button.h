#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button
{
public:
    // Constructor to initialize the button with size, position, text, and font
    Button(const sf::Vector2f size, sf::Vector2f position, const std::string &text, sf::Font &font);

    // Method to draw the button on the given window
    void draw(sf::RenderWindow &window);

    // Method to check if the button is clicked based on mouse position
    bool isClicked(const sf::Vector2f &mousePos);

    // Method to provide access to the button's shape for external modifications
    sf::RectangleShape &getButtonShape();

private:
    sf::RectangleShape button;
    sf::Text label;
};

#endif
