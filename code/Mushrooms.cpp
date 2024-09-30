/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
The MushroomManager and Shroom class definition.
*/

#include <algorithm>
#include <list>
#include <random>

#include "Mushrooms.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/** Base constructor from x,y coordinates */
Shroom::Shroom(float x, float y)
{
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    const auto& size = FullTexOffset.getSize();

    this->setTexture(tex);
    this->setTextureRect(FullTexOffset);
    this->setOrigin(size.x / 2.f, size.y / 2.f);
    this->setPosition(x, y);
}

/** Constructor overload for Vector parameter*/
Shroom::Shroom(sf::Vector2f location) : Shroom{location.x, location.y} {}

/**
 * Every mushroom starts with health=4, and can change to 3 levels of damage
 * before being destroyed;
 * @return remaining health
 */
int Shroom::damage()
{
    if (m_health <= 0) {
        return 0;
    }

    m_health -= 1;

    // Step through the textures for different damage levels,
    switch (m_health) {
    case 3:
        this->setTextureRect(Damage3TexOffset);
        break;
    case 2:
        this->setTextureRect(Damage2TexOffset);
        break;
    case 1:
        this->setTextureRect(Damage1TexOffset);
        break;
    case 0:
        // nothing to do, dead now. will get removed by manager
        break;
    default:
        throw std::runtime_error("Exhuastive switch failure: shroom.m_health=" + m_health);
        break;
    }

    return m_health;
}

sf::Vector2f Shroom::getRightEdge() const
{
    const sf::FloatRect& size = this->getLocalBounds();
    const sf::Vector2f& center = this->getPosition();
    return sf::Vector2f(center.x + size.width / 2.f, center.y);
}

sf::Vector2f Shroom::getLeftEdge() const
{
    const sf::FloatRect& size = this->getLocalBounds();
    const sf::Vector2f& center = this->getPosition();
    return sf::Vector2f(center.x - size.width / 2.f, center.y);
}

/**
 * Manager constructor initializes the members and
 * creates 30 mushroom sprites randomly scattered in the given bounds.
 *
 * The random number generate needs to be initalized with a true random device, or a seed value.
 *
 * @param bounds Rectangle where mushrooms should be placed
 */
MushroomManager::MushroomManager(sf::FloatRect bounds)
    : m_bounds(bounds), m_rng(std::random_device{}())
{
    // Need a random distribution aligned in the 30x30 grid
    auto x_range = static_cast<int>(m_bounds.width / Game::GridSize) - 1;
    auto y_range = static_cast<int>(m_bounds.height / Game::GridSize) - 1;
    std::uniform_int_distribution<int> random_x(0, x_range);
    std::uniform_int_distribution<int> random_y(0, y_range);

    // Create 30 mushroom sprites in random locations
    for (size_t i = 0; i < 30; ++i) {
        // random grid cells need to be offset so they refer to the center
        const float xPos = m_bounds.left + Game::GridSize * random_x(m_rng) + Game::GridSize / 2.f;
        const float yPos = m_bounds.top + Game::GridSize * random_y(m_rng) + Game::GridSize / 2.f;
        // Create and add to list in-place
        m_shrooms.emplace_back(xPos, yPos);
    }
}

/** Constant reference getter prevents modification */
const std::list<Shroom>& MushroomManager::getShrooms() const
{
    return m_shrooms;
}

/** Implement virtual method from Drawable so MushroomManager can be drawn by the Engine */
void MushroomManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // only draw currently active mushrooms
    for (const auto& shroom : m_shrooms) {
        target.draw(shroom, states);
    }
}

/** Remove any mushrooms that the spider intersects with */
bool MushroomManager::checkSpiderCollision(sf::FloatRect spider)
{
    // construct lambda predicate
    auto isHit = [&](const Shroom& s) {
        return spider.intersects(s.getGlobalBounds());
    };
    // find first intersecting mushroom
    auto hit_it = std::find_if(m_shrooms.begin(), m_shrooms.end(), isHit);
    // remove if found
    if (hit_it != m_shrooms.end()) {
        m_shrooms.erase(hit_it);
        return true;
    }
    return false;
}

/** Damage any mushroom hit by a laser beam.
 * @return true if a mushroom was hit
 */
bool MushroomManager::checkLaserCollision(sf::FloatRect laser)
{
    // construct lambda predicate
    auto isHit = [&](const Shroom& s) {
        return laser.intersects(s.getGlobalBounds());
    };

    // find the first intersecting mushroom (probably only 1 or none)
    auto hit_it = std::find_if(m_shrooms.begin(), m_shrooms.end(), isHit);

    // found one
    if (hit_it != m_shrooms.end()) {
        // Damage the mushroom to change it's texture,
        int remaining_health = hit_it->damage();
        // and delete if it was destroyed
        if (remaining_health <= 0) {
            m_shrooms.erase(hit_it);
        }
        return true;
    }
    // laser didn't hit anything
    return false;
}

/** Adds a mushroom the list list at a specific location.
 * Used by centipede class with split.
 */
void MushroomManager::addMushroom(sf::Vector2f location)
{
    m_shrooms.emplace_back(location);
}
