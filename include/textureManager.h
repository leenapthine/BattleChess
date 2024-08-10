#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager
{
public:
    bool loadTexture(const std::string &name, const std::string &filePath);
    sf::Texture *getTexture(const std::string &name);

private:
    std::map<std::string, sf::Texture> textures;
};

#endif // TEXTURE_MANAGER_H