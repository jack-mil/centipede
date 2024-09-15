#include "Laser.hpp"
#include "TextureHolder.hpp"

Laser::Laser() {
    sf::Vector2f size(3.0, 20.0);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(255, 0, 0, 255));

    m_shape.setOrigin(size.x / 2.0, size.y / 2.0);
    // m_sprite.setTexture(TextureHolder::GetTexture("graphics/"))
}

void Laser::shoot(sf::Vector2f start) {
    shoot(start.x, start.y);
}

void Laser::shoot(float xStart, float yStart) {
    m_active = true;
    m_pos.x = xStart;
    m_pos.y = yStart;

    m_shape.setPosition(m_pos);
}

void Laser::update(float deltaTime) {
    m_pos.y -= m_speed * deltaTime;

    m_shape.setPosition(m_pos);

    if (m_pos.y < 0) {
        m_active = false;
    }
}