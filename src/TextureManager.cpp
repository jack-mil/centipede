#include <cassert>
#include <exception>

#include "SFML/Graphics.hpp"

#include "TextureManager.hpp"

TextureManager* TextureManager::m_s_Instance = nullptr;

/** Constructor sets up the static reference. */
TextureManager::TextureManager() : m_texCache()
{
    // assert prevent's multiple TextureManagers for being created
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

/**
 * @brief Return a texture reference, loading it from a file if necessary
 *
 * This is a static method that makes it easy for any code to get a texture reference.
 * @param path the texture to load
 * @return sf::Texture&
 */
const sf::Texture& TextureManager::GetTexture(const char* path)
{
    // convert to string
    std::string filename{path};
    // reference to mapping in instance object
    auto& texture_cache = m_s_Instance->m_texCache;

    // Check mapping for the filename, return value if found (C++17 init statement syntax)
    if (auto got = texture_cache.find(filename); got != texture_cache.end())
    {

        return got->second;
    }
    else
    {
        // File not loaded yet!
        // Create a new key value pair using the filename
        auto& texture = texture_cache[filename];

        if (!texture.loadFromFile(filename))
        {
            // If file can't be found, abort
            throw std::runtime_error("Could not load file: " + filename);
        }
        // texture is valid and loaded
        return texture;
    }
}