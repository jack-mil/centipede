/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Laser object definition. These are instantiated once at runtime,
then activated/moved when needed.

The original Centipede game only allowed one Laser on the screen at a time,
but the requirements for this assignment specifically asked for multiple.

TODO: I will probably change how this works after submitting.
*/
#include "SFML/Graphics.hpp"

#include "Laser.hpp"

/**
 * Construct a new Laser object.
 * Set the size and color, and set origin to center.
 */
Laser::Laser() : m_shape{Laser::Size}
{
    m_shape.setFillColor(Laser::Color);
    m_shape.setOrigin(Laser::Size.x / 2.f, Laser::Size.y / 2.f);
}

/** Update sprite position based on elapsed seconds. */
void Laser::update(float deltaTime)
{
    // Don't do anything if not active
    if (!m_active)
    {
        return;
    }

    // distance traveled (up)
    const float distance = -Laser::Speed * deltaTime;

    m_shape.move(0, distance);

    // deactivate when hitting the top of the screen
    if (m_shape.getPosition().y < 0)
    {
        m_active = false;
    }
}

/** Only draw an active laser to the scene */
void Laser::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_active)
    {
        target.draw(m_shape, states);
    }
}

/**
 * Make this Laser active, and set it's position to (x,y)
 * @param x pos of start
 * @param y pos of start
 */
void Laser::shoot(float x, float y)
{
    m_active = true;

    m_shape.setPosition(x, y);
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

/** Return the boundary collider of this laser object. */
sf::FloatRect Laser::getCollider() const
{
    return m_shape.getGlobalBounds();
}

bool Laser::isActive() const
{
    return m_active;
}

void Laser::deactivate()
{
    m_active = false;
}