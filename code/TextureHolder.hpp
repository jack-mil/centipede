#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * @brief Singleton central cache of textures
 *
 * Stolen from Chapter 18 code.
 */
class TextureHolder {
private:
    // A map container from the STL,
    // that holds related pairs of String and Texture
    std::unordered_map<std::string, sf::Texture> m_Textures;

    // A pointer of the same type as the class itself
    // the one and only instance
    static TextureHolder *m_s_Instance;

public:
    TextureHolder();
    static sf::Texture& GetTexture(std::string const& filename);
};
