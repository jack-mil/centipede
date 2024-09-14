#include <assert.h>
#include <iostream>

#include "TextureHolder.hpp"

TextureHolder *TextureHolder::m_s_Instance = nullptr;

/**
 * @brief Construct a new Texture Holder:: Texture Holder object
 *
 */
TextureHolder::TextureHolder()
{
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

/**
 * @brief Return a texture, loading it from a file if necessary
 *
 * @param filename the texture to load
 * @return sf::Texture&
 */
sf::Texture &TextureHolder::GetTexture(std::string const &filename)
{
    // Get a reference to m_Textures using m_S_Instance
    auto &texture_cache = m_s_Instance->m_Textures;
    // auto is the equivalent of map<string, Texture>

    // Create an iterator to hold a key-value-pair (kvp)
    // and search for the required kvp
    // using the passed in file name
    auto keyValuePair = texture_cache.find(filename);
    // auto is equivalent of map<string, Texture>::iterator

    // Did we find a match?
    if (keyValuePair != texture_cache.end())
    {
        // Yes
        // Return the texture,
        // the second part of the kvp, the texture
        return keyValuePair->second;
    }
    else
    {
        // File name not found
        // Create a new key value pair using the filename
        auto &texture = texture_cache[filename];
        // Load the texture from file in the usual way
        bool succeed = texture.loadFromFile(filename);
        if (!succeed)
        {
            std::cerr << "Loading file" << filename << "failed.";
        }

        // Return the loaded texture
        return texture;
    }
}