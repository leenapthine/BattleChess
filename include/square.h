#ifndef SQUARE_H
#define SQUARE_H

#include <SFML/Graphics.hpp>

class Square
{
public:
    Square(const sf::Vector2f &position, const sf::Color &color);
    void setPosition(const sf::Vector2f &position);
    void setColor(const sf::Color &color);
    sf::RectangleShape getShape() const;
    void draw(sf::RenderWindow &window);
    void setHighlight(bool highlight, sf::Color color = sf::Color::Yellow);
    bool getHighlight() const;

private:
    sf::RectangleShape shape;
    bool highlighted;
    sf::Color highlightColor;
};

#endif // SQUARE_H
