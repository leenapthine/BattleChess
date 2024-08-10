#include "textureManager.h"
#include <stdexcept>

bool TextureManager::loadTexture(const std::string &name, const std::string &filePath)
{
    sf::Texture texture;
    if (!texture.loadFromFile(filePath))
    {
        throw std::runtime_error("Failed to load " + filePath);
        return false;
    }
    textures[name] = std::move(texture);
    return true;
}

sf::Texture *TextureManager::getTexture(const std::string &name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return &it->second;
    }
    return nullptr;
}
