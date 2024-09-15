#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * Central cache of textures.
 * Stolen from Chapter 18 code.
 */
class TextureManager {
private:
    // A map container from the STL,
    // that holds related pairs of String and Texture
    std::unordered_map<std::string, sf::Texture> m_textures;

    /** Pointer of the same type as the class itself
     * the one and only instance.
     * */
    static TextureManager *m_s_Instance;

public:
    /** Constructor sets up the static reference. */
    TextureManager();
    static sf::Texture& GetTexture(std::string const& filename);
};
