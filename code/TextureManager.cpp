#include <assert.h>
#include <iostream>

#include "TextureManager.hpp"

TextureManager *TextureManager::m_s_Instance = nullptr;

/**
 * Only one TextureManager should every be created.
 * Constructor stores a static class reference to the first instance.
 */
TextureManager::TextureManager():m_textures(){
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

/**
 * @brief Return a texture reference, loading it from a file if necessary
 *
 * @param filename the texture to load
 * @return sf::Texture&
 */
sf::Texture& TextureManager::GetTexture(const std::string& filename) {
    // Get a reference to m_textures using m_S_Instance
    auto& texture_cache = m_s_Instance->m_textures;
    // auto is the equivalent of map<string, Texture>

    // Create an iterator to hold a key-value-pair (kvp)
    // and search for the required kvp
    // using the passed in file name
    auto keyValuePair = texture_cache.find(filename);
    // auto is equivalent of map<string, Texture>::iterator

    // Did we find a match?
    if (keyValuePair != texture_cache.end()) {
        // Yes
        // Return the texture,
        // the second part of the kvp, the texture
        return keyValuePair->second;
    } else {
        // File name not found
        // Create a new key value pair using the filename
        auto& texture = texture_cache[filename];
        // Load the texture from file in the usual way
        bool succeed = texture.loadFromFile(filename);
        if (!succeed) {
            std::cerr << "Loading  \"" << filename << "\" failed." << std::endl;
        }

        // Return the loaded texture reference
        return texture;
    }
}