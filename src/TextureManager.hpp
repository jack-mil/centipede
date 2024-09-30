#pragma once

#include <unordered_map>

#include "SFML/Graphics.hpp"

/**
 * Central cache of textures.
 * Prevents loading files from disks more than once,
 * and allows many sprites to share the same texture.
 * Modified from Chapter 18 code.
 */
class TextureManager
{
  private:
    /** Pointer of the same type as the class itself
     *  the one and only instance.
     * */
    static TextureManager* m_s_Instance;

    /** Mapping of filenames to Texture objects */
    std::unordered_map<std::string, sf::Texture> m_texCache;

    // const sf::Image m_spriteSheet;

  public:
    /**
     * Only one TextureManager should every be created.
     * Constructor stores a static class reference to the first instance.
     */
    TextureManager();

    /**
     * @brief Return a texture reference, loading it from a file if necessary
     *
     * This is a static method that makes it easy for any code to get a texture reference.
     * @param filename the texture to load
     * @return sf::Texture&
     */
    static const sf::Texture& GetTexture(const char* path);
};
