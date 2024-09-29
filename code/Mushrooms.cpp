/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
The MushroomManager class definition.
It creates a collection of mushrooms, and handles their state throughout the game.
*/
#include <exception>

#include "Mushrooms.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/**
 * Default constructor initializes the members but doesn't create any sprites
 * The random number generate needs to be initalized with a true random device, or a seed value.
 */
MushroomManager::MushroomManager(sf::FloatRect bounds)
    : m_bounds(bounds), m_rng(std::random_device{}())
{
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
void MushroomManager::spawn()
{
    // Every sprite will use the same texture
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    const sf::Vector2i& size = FullTexOffset.getSize();

    // Need a random distribution in the 30x30 grid
    auto x_range = static_cast<int>(m_bounds.width / Game::GridSize) - 1;
    auto y_range = static_cast<int>(m_bounds.height / Game::GridSize) - 1;
    std::uniform_int_distribution<int> random_x(0, x_range);
    std::uniform_int_distribution<int> random_y(0, y_range);

    // Create 30 mushroom sprites with the same texture and random location
    for (size_t i = 0; i < 30; ++i) {
        // Create a new sprite using the mushroom texture offset.
        Shroom shroom{tex, FullTexOffset};
        // change origin to center instead of top/left
        shroom.sprite.setOrigin(size.x / 2.f, size.y / 2.f);
        // location is random, but aligned to 8x8 grid
        const float xPos = m_bounds.left + Game::GridSize * random_x(m_rng) + size.x / 2.f;
        const float yPos = m_bounds.top + Game::GridSize * random_y(m_rng) + size.y / 2.f;
        shroom.sprite.setPosition(xPos, yPos);
        m_shrooms.push_back(std::move(shroom));
    }
}

void MushroomManager::draw(sf::RenderTarget& target)
{
    for (const auto& shroom : m_shrooms) {
        if (shroom.active) {
            target.draw(shroom.sprite);
        }
    }
}

void MushroomManager::damage(Shroom& shroom)
{
    if (!shroom.active) {
        return;
    }

    if (shroom.health == 0) {
        return;
    }

    shroom.health--;

    // Step through the textures for different damage levels,
    //  and de-activate when fully destroyed.
    switch (shroom.health) {
    case 3:
        shroom.sprite.setTextureRect(Damage3TexOffset);
        break;
    case 2:
        shroom.sprite.setTextureRect(Damage2TexOffset);
        break;
    case 1:
        shroom.sprite.setTextureRect(Damage1TexOffset);
        break;
    case 0:
        shroom.active = false;
        break;
    default:
        throw std::runtime_error("Exhuastive switch failure: shroom.health=" + shroom.health);
        break;
    }
}

/** Check all mushrooms to see if the spider is colliding */
bool MushroomManager::checkSpiderCollision(sf::FloatRect spider)
{
    for (auto& shroom : m_shrooms) {
        if (shroom.active && spider.intersects(shroom.sprite.getGlobalBounds())) {
            shroom.active = false;
            return true;
        }
    }
    return false;
}

/** Damage any mushroom hit by a laser beam */
bool MushroomManager::checkLaserCollision(sf::FloatRect laser)
{
    for (auto& shroom : m_shrooms) {
        if (shroom.active && laser.intersects(shroom.sprite.getGlobalBounds())) {
            MushroomManager::damage(shroom);
            return true;
        }
    }
    return false;
}

void MushroomManager::addMushroom(sf::Vector2f location)
{
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    const auto& size = FullTexOffset.getSize();
    auto& newShroom = m_shrooms.emplace_back(tex, FullTexOffset);
    newShroom.sprite.setPosition(location);
    newShroom.sprite.setOrigin(size.x / 2.f, size.y / 2.f);
}

sf::Vector2f MushroomManager::Shroom::getRightEdge() const
{
    const sf::FloatRect& size = sprite.getLocalBounds();
    const sf::Vector2f& center = sprite.getPosition();
    return sf::Vector2f(center.x + size.width / 2.f, center.y);
}

sf::Vector2f MushroomManager::Shroom::getLeftEdge() const
{
    const sf::FloatRect& size = sprite.getLocalBounds();
    const sf::Vector2f& center = sprite.getPosition();
    return sf::Vector2f(center.x - size.width / 2.f, center.y);
}