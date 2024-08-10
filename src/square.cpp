// Filename: square.cpp
// Description: Implementation of the Square class, which represents individual squares on the chessboard and handles their visual representation,
// including highlighting for valid moves or selected squares.

// Main Classes:
// - Square: Represents a single square on the chessboard, managing its position, color, and highlighting state.

// Special Features or Notes:
// - The class supports highlighting to visually indicate valid moves, captures, or selected squares.
// - Highlighting is indicated by a colored border around the square, which is drawn only when the square is in a highlighted state.

// Usage or Context:
// - This file implements the visual logic for squares on the chessboard
//   for rendering the board and indicating valid moves or selections during gameplay.

#include "square.h"
#include "globals.h"

// Constructor that initializes the square with a given position and color.
Square::Square(const sf::Vector2f &position, const sf::Color &color)
    : highlighted(false), highlightColor(sf::Color::Yellow), position(position)
{
    shape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    shape.setPosition(position);
    shape.setFillColor(color);
}

// Sets the position of the square on the chessboard.
void Square::setPosition(const sf::Vector2f &position)
{
    shape.setPosition(position);
}

sf::Vector2f Square::getPosition() const
{
    return shape.getPosition();
}

// Sets the color of the square.
void Square::setColor(const sf::Color &color)
{
    shape.setFillColor(color);
}

// Returns the shape of the square for drawing purposes.
sf::RectangleShape Square::getShape() const
{
    return shape;
}

// Draws the square onto the provided render window, applying any highlighting if necessary.
void Square::draw(sf::RenderWindow &window)
{
    if (highlighted)
    {
        shape.setOutlineThickness(5);
        shape.setOutlineColor(highlightColor);
    }
    else
    {
        shape.setOutlineThickness(0);
    }
    window.draw(shape);
}

// Sets the highlighting state and color for the square.
void Square::setHighlight(bool highlight, sf::Color color)
{
    highlighted = highlight;
    highlightColor = color;
}

// Returns the current highlighting state of the square.
bool Square::getHighlight() const
{
    return highlighted;
}
