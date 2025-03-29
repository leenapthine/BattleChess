#ifndef PIECE_H
#define PIECE_H

// Filename: piece.h
// Description: Declaration of the Piece class, serving as the base class for all chess pieces. It defines common attributes and behaviors for pieces in the chess game.

// Main Classes:
// - Piece: An abstract base class that represents a generic chess piece and provides common functionality for all pieces.

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "square.h"

class Piece
{
public:
    enum class Color
    {
        White,
        Black
    };

    // Constructor that initializes the piece with a texture, initial position, and color. Sets the sprite's position.
    Piece(int id, const sf::Texture &texture, const sf::Vector2f &initialPosition, Color color)
        : id(id), texture(texture), sprite(texture), position(initialPosition), color(color)
    {
        sprite.setPosition(position);
    }

    bool operator!=(const Piece &other) const
    {
        return (this->getType() != other.getType() ||
                this->getPosition() != other.getPosition() ||
                this->getColor() != other.getColor());
    }

    // Virtual destructor to ensure proper cleanup of derived classes.
    virtual ~Piece() = default;
    virtual std::unique_ptr<Piece> clone() const = 0;

    // Virtual function to get the type of the piece.
    virtual std::string getType() const = 0;

    // return id
    int getId() const { return id; }

    // Returns a reference to the piece's sprite for rendering or modification.
    sf::Sprite &getSprite() { return sprite; }

    // Sets the piece's position and updates the sprite's position accordingly.
    void setPosition(const sf::Vector2f &pos)
    {
        position = pos;
        sprite.setPosition(position);
    }

    // Returns the current position of the piece.
    const sf::Vector2f &getPosition() const { return position; }

    // Returns the color of the piece.
    Color getColor() const { return color; }

    // Sets the color of the piece.
    void setColor(Color newColor) { color = newColor; }

    void print(std::ostream &os) const
    {
        os << "Piece at position: (" << getPosition().x << ", " << getPosition().y << "), Type: " << getType() << ", Color: " << (getColor() == Color::White ? "White" : "Black");
    }

    // Draws the piece on the given window.
    void draw(sf::RenderWindow &window) { window.draw(sprite); }

    // Set the texture fo the sprite
    void setTexture(const sf::Texture &texture)
    {
        sprite.setTexture(texture);
    }

    // Return a const reference to the texture
    const sf::Texture &getTexture() const
    {
        return texture;
    }

    // Virtual function to determine if the piece can move to the target position. Must be implemented by derived classes.
    virtual bool canMoveTo(const sf::Vector2f &target, const std::vector<std::unique_ptr<Piece>> &pieces) const = 0;

    // Virtual function to highlight all valid moves for the piece on the chessboard. Must be implemented by derived classes.
    virtual void highlightValidMoves(std::vector<std::vector<Square>> &board, const std::vector<std::unique_ptr<Piece>> &pieces) const = 0;

    // Moves the piece to the target position and removes any opponent piece at that position. Updates the position of the capturing piece.
    virtual void capture(const sf::Vector2f &target, std::vector<std::unique_ptr<Piece>> &pieces)
    {
        {
            auto it = std::remove_if(pieces.begin(), pieces.end(),
                                     [&](const std::unique_ptr<Piece> &piece)
                                     {
                                         return piece->getPosition() == target && piece->getColor() != getColor();
                                     });
            pieces.erase(it, pieces.end());
        }
        setPosition(target);
    }

    bool isStunned() const { return stunned; }
    void setStunned(bool value) { stunned = value; }
    bool isStone() const { return stone; }
    void setStone(bool value) { stone = value; }

protected:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    Color color;
    bool stunned = false;
    bool stone = false;
    int id;
};

// Declare operator<< for Piece outside the class
std::ostream &operator<<(std::ostream &os, const Piece &piece);

#endif
