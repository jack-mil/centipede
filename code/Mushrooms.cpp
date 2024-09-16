/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
The MushroomManager class definition.
It creates a collection of mushrooms, and handles their state throughout the game.
*/
#include "Mushrooms.hpp"
#include "TextureManager.hpp"

/**
 * Default constructor initializes the members but doesn't create any sprites
 * The random number generate needs to be initalized with a true random device, or a seed value.
 */
MushroomManager::MushroomManager() : m_shrooms(), m_rng(std::random_device{}()) {
    // Reserve space for 30 sprites to avoid reallocations
    m_shrooms.reserve(30);
}

/**
 * Create 30 mushroom sprites randomly scattered in the given bounds.
 * @param bounds Rectangle where mushrooms should be placed
 */
void MushroomManager::spawn(sf::FloatRect bounds) {
    // Copy bounds to member
    m_bounds.top = bounds.top;
    m_bounds.left = bounds.left;
    m_bounds.width = bounds.width;
    m_bounds.height = bounds.height;

    // Every sprite will use the same texture
    const auto& tex = TextureManager::GetTexture("graphics/mushroom.png");
    const auto& texSize = tex.getSize();

    // Need a random distribution for x and y axis
    std::uniform_int_distribution<int> random_x(m_bounds.left, m_bounds.left + m_bounds.width);
    std::uniform_int_distribution<int> random_y(m_bounds.top, m_bounds.top + m_bounds.height);

    // Create 30 mushroom sprites with the same texture and random location
    for (int i = 0; i < 30; ++i) {
        sf::Sprite sprite(tex);
        sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
        sprite.setPosition(random_x(m_rng), random_y(m_rng));
        m_shrooms.push_back(sprite);
    }
}