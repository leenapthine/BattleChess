#include "button.h"

// Constructor implementation to initialize the button's size, position, text, and font
Button::Button(const sf::Vector2f size, sf::Vector2f position, const std::string &text, sf::Font &font)
{
    // Set the button size and position
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(sf::Color::White); // Default color

    // Set the label's font, text, and appearance
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(24);
    label.setFillColor(sf::Color::Black);

    // Center the text within the button
    sf::FloatRect textRect = label.getLocalBounds();
    label.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    label.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

// Method to draw the button and its label on the provided window
void Button::draw(sf::RenderWindow &window)
{
    window.draw(button);
    window.draw(label);
}

// Method to check if the button is clicked based on the provided mouse position
bool Button::isClicked(const sf::Vector2f &mousePos)
{
    return button.getGlobalBounds().contains(mousePos);
}

// Method to provide access to the button's shape
sf::RectangleShape &Button::getButtonShape()
{
    return button;
}