#ifndef NETWORK_H
#define NETWORK_H

#include "piece.h"
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

struct PieceData
{
    int id;                // Unique ID for the piece
    sf::Vector2f position; // Position on the board
};

// Operator overloads for SFML packet
sf::Packet &operator<<(sf::Packet &packet, const PieceData &data);
sf::Packet &operator>>(sf::Packet &packet, PieceData &data);

std::unique_ptr<sf::TcpSocket> initialConnect();

void sendPacket(std::unique_ptr<sf::TcpSocket> &socket, const std::vector<std::unique_ptr<Piece>> &currentPack);
void receivePacket(std::unique_ptr<sf::TcpSocket> &socket, std::vector<std::unique_ptr<Piece>> &currentPack, TextureManager &textureManager, Piece::Color color);

#endif