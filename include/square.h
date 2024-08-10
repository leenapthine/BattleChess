#ifndef SQUARE_H
#define SQUARE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Square
{
public:
    // Constructor to initialize the position and color
    Square(const sf::Vector2f &position, const sf::Color &color);

    // Set the position of the square
    void setPosition(const sf::Vector2f &position);

    // Get the position of the square
    sf::Vector2f getPosition() const;

    // Set the color of the square
    void setColor(const sf::Color &color);

    // Get the shape of the square for drawing
    sf::RectangleShape getShape() const;

    // Draw the square onto the render window
    void draw(sf::RenderWindow &window);

    // Set highlighting state and color
    void setHighlight(bool highlight, sf::Color color = sf::Color::Yellow);

    // Get the current highlighting state
    bool getHighlight() const;

private:
    sf::RectangleShape shape; // The shape representing the square
    bool highlighted;         // Whether the square is highlighted
    sf::Color highlightColor; // Color of the highlight
    sf::Vector2f position;    // Position of the square
};

#endif // SQUARE_H
