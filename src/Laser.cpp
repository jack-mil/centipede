/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Laser object definition. These are instantiated once at runtime,
then activated/moved when needed.
*/
#include <SFML/Graphics.hpp>

#include "Laser.hpp"

/**
 * Construct a new Laser object.
 * Set the size and color, and set origin to center.
 */
Laser::Laser() : m_pos(), m_shape()
{
    m_shape.setSize(Laser::Size);
    m_shape.setFillColor(Laser::Color);
    m_shape.setOrigin(Laser::Size.x / 2.f, Laser::Size.y / 2.f);
}

/**
 * Make this Laser active, and set it's position to (x,y)
 * @param x pos of start
 * @param y pos of start
 */
void Laser::shoot(float x, float y)
{
    active = true;
    m_pos.x = x;
    m_pos.y = y;

    m_shape.setPosition(m_pos);
}

/**
 * Make this Laser active, and set it's position to `start`.
 * Overload for vector start position
 * @param start  position to start from
 */
void Laser::shoot(sf::Vector2f start)
{
    shoot(start.x, start.y);
}

/** Update sprite position based on elapsed seconds. */
void Laser::update(float deltaTime)
{
    if (!active) {
        return;
    }
    m_pos.y -= Laser::Speed * deltaTime;

    m_shape.setPosition(m_pos);

    if (m_pos.y < 0) {
        active = false;
    }
}

/** Return the boundary collider of this laser object. */
sf::FloatRect Laser::getCollider() const
{
    return m_shape.getGlobalBounds();
}