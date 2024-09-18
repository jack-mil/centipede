/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
The MushroomManager class definition.
It creates a collection of mushrooms, and handles their state throughout the game.
*/
#include "Mushrooms.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/**
 * Default constructor initializes the members but doesn't create any sprites
 * The random number generate needs to be initalized with a true random device, or a seed value.
 */
MushroomManager::MushroomManager() : m_shrooms(), m_rng(std::random_device{}()) {
    // Reserve space for 30 sprites to avoid reallocations
    m_shrooms.reserve(30);
    // m_shroomShader.loadFromFile("shaders/mushroom.frag", sf::Shader::Fragment);
    // m_shroomShader.setUniform("sheet", TextureManager::GetTexture("graphics/sprite_sheet.png"));
    // m_shroomShader.setUniform("rect1", sf::Glsl::Vec4(104,107,8,8));
    // m_shroomShader.setUniform("rect2", sf::Glsl::Vec4(104,123,8,8));
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
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    const sf::IntRect texOffset(104, 107, 8, 8); // where the mushroom is
    const auto& texSize = texOffset.getSize();

    // Need a random distribution in the 30x30 grid
    std::uniform_int_distribution<int> random_x(0, 29);
    std::uniform_int_distribution<int> random_y(0, 29);

    // Create 30 mushroom sprites with the same texture and random location
    for (int i = 0; i < 30; ++i) {
        // Create a new sprite using the mushroom texture offset.
        sf::Sprite sprite(tex, texOffset);
        // location is random, but aligned to 8x8 grid
        const float xPos = m_bounds.left + Game::GridSize * random_x(m_rng);
        const float yPos = m_bounds.top + Game::GridSize * random_y(m_rng);
        sprite.setPosition(xPos, yPos);
        m_shrooms.push_back(sprite);
    }
}

void MushroomManager::drawAll(sf::RenderWindow& window) {
    for (const auto& shroom : m_shrooms) {
        window.draw(shroom);
    }
}