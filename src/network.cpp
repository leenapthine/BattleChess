#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include "globals.h"
#include "textureManager.h"
#include "utility.h"
#include "square.h"
#include "network.h"
#include "piece.h"
#include "pieceFactory.h"
#include "pawn.h"

// Implement the operator overload for serialization
sf::Packet &operator<<(sf::Packet &packet, const PieceData &data)
{
    return packet << data.id << data.position.x << data.position.y;
}

// Implement the operator overload for deserialization
sf::Packet &operator>>(sf::Packet &packet, PieceData &data)
{
    return packet >> data.id >> data.position.x >> data.position.y;
}

std::unique_ptr<sf::TcpSocket> initialConnect()
{
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::TcpListener listener;
    std::unique_ptr<sf::TcpSocket> socket = std::make_unique<sf::TcpSocket>();
    char connectionType;
    char buffer[8192]; // Increased buffer size
    std::size_t received;
    std::string text = "Connected to: ";

    std::cout << "Enter (s) for server or (c) for client: " << std::endl;
    std::cin >> connectionType;

    if (connectionType == 's')
    {
        isPlayerWhite = true;

        if (listener.listen(2000) != sf::Socket::Done)
        {
            std::cerr << "Error: Unable to listen on port 2000" << std::endl;
            exit(EXIT_FAILURE); // Ensure the program stops if the server fails to start.
        }

        std::cout << "Server is listening..." << std::endl;
        if (listener.accept(*socket) != sf::Socket::Done)
        {
            std::cerr << "Error: Unable to accept connection" << std::endl;
            exit(EXIT_FAILURE); // Ensure the program stops if the connection fails.
        }

        text += "Server";
    }
    else if (connectionType == 'c')
    {
        isPlayerWhite = false;

        if (socket->connect(ip, 2000) != sf::Socket::Done)
        {
            std::cerr << "Error: Unable to connect to server" << std::endl;
            exit(EXIT_FAILURE); // Ensure the program stops if the connection fails.
        }

        text += "Client";
    }
    else
    {
        std::cerr << "Invalid connection type entered" << std::endl;
        exit(EXIT_FAILURE); // Ensure the program stops if the input is invalid.
    }

    if (socket->send(text.c_str(), text.length() + 1) != sf::Socket::Done)
    {
        std::cerr << "Error: Unable to send data" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (socket->receive(buffer, sizeof(buffer) - 1, received) != sf::Socket::Done)
    {
        std::cerr << "Error: Unable to receive data" << std::endl;
        exit(EXIT_FAILURE);
    }
    buffer[received] = '\0'; // Null-terminate the buffer

    std::cout << text << std::endl;
    std::cout << buffer << std::endl;

    return socket; // Return the unique_ptr to manage the socket's lifetime.
}

void sendPacket(std::unique_ptr<sf::TcpSocket> &socket, const std::vector<std::unique_ptr<Piece>> &currentPack)
{
    sf::Packet packet;

    // Serialize the number of pieces
    packet << static_cast<sf::Uint32>(currentPack.size());

    // Serialize each piece's ID, type, and position
    for (const auto &piece : currentPack)
    {
        int id = piece->getId();
        std::string type = piece->getType();
        sf::Vector2f position = piece->getPosition();
        bool isStone = piece->isStone();
        bool isStunned = piece->isStunned();

        // Serialize the data to the packet
        packet << id << type << position.x << position.y << isStone << isStunned;
    }

    // Send the packet
    if (socket->send(packet) != sf::Socket::Done)
    {
        std::cerr << "Error: Unable to send the packet" << std::endl;
    }
}

void receivePacket(std::unique_ptr<sf::TcpSocket> &socket, std::vector<std::unique_ptr<Piece>> &currentPack, TextureManager &textureManager, Piece::Color color)
{
    sf::Packet packet;
    socket->setBlocking(false);

    sf::Socket::Status status = socket->receive(packet);
    if (status == sf::Socket::Disconnected)
    {
        std::cerr << "Disconnected from the server" << std::endl;
        return;
    }
    else if (status == sf::Socket::NotReady)
    {
        // std::cout << "Socket not ready for receiving data" << std::endl;
        return;
    }
    else if (status != sf::Socket::Done)
    {
        std::cerr << "Error: Unable to receive the packet" << std::endl;
        return;
    }

    // Deserialize the number of pieces
    sf::Uint32 numPieces;
    packet >> numPieces;

    // A set to track the IDs of pieces received in the packet
    std::set<int> receivedPieceIDs;

    // Iterate over the received pieces
    for (sf::Uint32 i = 0; i < numPieces; ++i)
    {
        int id;
        std::string type;
        sf::Vector2f position;
        bool isStone;
        bool isStunned;

        // Deserialize the piece data
        packet >> id >> type >> position.x >> position.y >> isStone >> isStunned;

        // Add the ID to the set of received IDs
        receivedPieceIDs.insert(id);

        // Find the piece by its ID and update its position
        bool found = false;
        for (auto &piece : currentPack)
        {
            if (piece->getId() == id)
            {
                piece->setPosition(position);
                piece->setStunned(isStunned);
                piece->setStone(isStone);
                found = true;
                break;
            }
        }

        // If the piece was not found in currentPack, create and add it
        if (!found)
        {
            auto newPiece = createPiece(id, type, textureManager, position, color);
            newPiece->setStunned(isStunned);
            newPiece->setStone(isStone);
            currentPack.push_back(std::move(newPiece));
        }
    }

    // Remove pieces from currentPack that are not in the received set
    currentPack.erase(std::remove_if(currentPack.begin(), currentPack.end(),
                                     [&receivedPieceIDs](const std::unique_ptr<Piece> &piece)
                                     {
                                         return receivedPieceIDs.find(piece->getId()) == receivedPieceIDs.end();
                                     }),
                      currentPack.end());

    std::cout << "Packet received and currentPack positions updated successfully!" << std::endl;
}
