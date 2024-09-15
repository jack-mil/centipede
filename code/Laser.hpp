#pragma once
#include <SFML/Graphics.hpp>

class Laser {
private:
    sf::Vector2f m_pos;
    // sf::Sprite m_sprite;

    float m_speed = 1200;

public:
    /** Drawable rectangle (color?) */
    sf::RectangleShape m_shape;
    /** Only draw active lasers */
    bool m_active = false;
    /** Construct a new Laser */
    Laser();
    /** Deactivate this Laser object */
    void stop();
    /** Fire a beam from the start position straight up */
    void shoot(float xStart, float yStart);
    void shoot(sf::Vector2f);

    /** Update sprite position based on elapsed seconds */
    void update(float deltaTime);
};