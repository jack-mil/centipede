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
    const sf::IntRect texOffset(104, 107, 8, 8); // where the mushroom is

    // Need a random distribution in the 30x30 grid
    auto x_range = static_cast<int>(m_bounds.width / Game::GridSize) - 1;
    auto y_range = static_cast<int>(m_bounds.height / Game::GridSize) - 1;
    std::uniform_int_distribution<int> random_x(0, x_range);
    std::uniform_int_distribution<int> random_y(0, y_range);

    // Create 30 mushroom sprites with the same texture and random location
    for (size_t i = 0; i < 30; ++i) {
        // Create a new sprite using the mushroom texture offset.
        Shroom shroom{tex, texOffset};
        // location is random, but aligned to 8x8 grid
        const float xPos = m_bounds.left + Game::GridSize * random_x(m_rng);
        const float yPos = m_bounds.top + Game::GridSize * random_y(m_rng);
        shroom.sprite.setPosition(xPos, yPos);
        m_shrooms.push_back(std::move(shroom));
    }
}

void MushroomManager::draw(sf::RenderWindow& target)
{
    for (const auto& shroom : m_shrooms) {
        if (shroom.active) {
            target.draw(shroom.sprite);
        }
    }
}

/** Change this mushroom to the damage sprite*/
void MushroomManager::damage(Shroom& shroom)
{
    if (shroom.health == 0) {
        return;
    }
    static const sf::IntRect lv1(152, 107, 8, 8);
    static const sf::IntRect lv2(136, 107, 8, 8);
    static const sf::IntRect lv3(120, 107, 8, 8);
    shroom.health--;
    switch (shroom.health) {
    case 3:
        shroom.sprite.setTextureRect(lv3);
        break;
    case 2:
        shroom.sprite.setTextureRect(lv2);
        break;
    case 1:
        shroom.sprite.setTextureRect(lv1);
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

sf::Vector2f MushroomManager::Shroom::getRightEdge() const
{
    const sf::FloatRect size = sprite.getLocalBounds();
    const sf::Vector2f& topLeft = sprite.getPosition();
    return sf::Vector2f(topLeft.x + size.width, topLeft.y + size.height / 2.0);
}

sf::Vector2f MushroomManager::Shroom::getLeftEdge() const
{
    const float height = sprite.getLocalBounds().height;
    const sf::Vector2f& topLeft = sprite.getPosition();
    return sf::Vector2f(topLeft.x, topLeft.y + height / 2.0);
}