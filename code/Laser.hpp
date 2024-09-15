#pragma once
#include <SFML/Graphics.hpp>

/**
 * Laser objects that can be recycled throughout the scene.
 * Instances that are not `Laser::active` should not be drawn or updated.
 */
class Laser {
private:
    // Static properties common to all lasers

    /** Laser speed in px/second*/
    inline static const float Speed = 1000;
    /** Color of all lasers (Red) */
    inline static const sf::Color Color = sf::Color::Red;
    /** Size of all lasers (px) */
    static inline const sf::Vector2f Size = sf::Vector2f(3.0, 20.0);
    /** Position of the center of the laser */
    sf::Vector2f m_pos;

public:
    /** Fire-rate of all Laser instances. */
    inline static const float fire_speed = 6.5;
    /** Shape to draw */
    sf::RectangleShape m_shape;
    /** Only draw active lasers. */
    bool active = false;
    /** Construct a new Laser. */
    Laser();
    /** Deactivate this Laser object. */
    void stop();
    /** Fire a beam from the start position straight up. */
    void shoot(float xStart, float yStart);
    /** Fire a beam from the start position straight . */
    void shoot(sf::Vector2f);

    /** Update sprite position based on elapsed seconds. */
    void update(float deltaTime);
};
