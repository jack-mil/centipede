#include "Laser.hpp"

/**
 * Construct a new Laser object.
 * Set the size and color, and set origin to center.
 */
Laser::Laser() : m_pos(), m_shape() {
    m_shape.setSize(Laser::Size);
    m_shape.setFillColor(Laser::Color);
    m_shape.setOrigin(Laser::Size.x / 2.0, Laser::Size.y / 2.0);
}

/**
 * Make this Laser active, and set it's position to (x,y)
 * @param x pos of start
 * @param y pos of start
 */
void Laser::shoot(float x, float y) {
    active = true;
    m_pos.x = x;
    m_pos.y = y;

    m_shape.setPosition(m_pos);
}
/**
 * Override for vector type.
 * @param start vector position to start from
 */
void Laser::shoot(sf::Vector2f start) {
    shoot(start.x, start.y);
}

/**
 * Move the beam straight up (negative y).
 *
 * Automatically deactivates when reaching the top.
 * @param deltaTime Elapsed time in seconds
 */
void Laser::update(float deltaTime) {
    if (!active) {
        return;
    }
    m_pos.y -= Laser::Speed * deltaTime;

    m_shape.setPosition(m_pos);

    if (m_pos.y < 0) {
        active = false;
    }
}
