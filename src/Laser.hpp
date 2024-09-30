/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Laser object declaration. These are activated when the <SPACE> key is pressed.
The actual Atari Centipede game only allowed one laser sprite on screen at a time.
I might go that route, it would simplify the fire-rate timing.
*/
#pragma once
#include <SFML/Graphics.hpp>

/**
 * Laser objects that can be recycled throughout the scene.
 * Instances that are not `Laser::active` should not be drawn or updated.
 */
class Laser : public sf::Drawable
{
  public:
    /** Fire-rate of all Laser instances (shots/second) */
    static constexpr double FireRate = 6.5;

    /** Construct a new Laser. */
    Laser();

    /**
     * Move the beam straight up (negative y).
     *
     * Automatically deactivates when reaching the top.
     * @param deltaTime Elapsed time in seconds
     */
    void update(float deltaTime);

    /**
     * Draw to the target
     * Implements sf::Drawable.draw
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * Make this Laser active, and set it's position to (x,y)
     * @param x pos of start
     * @param y pos of start
     */
    void shoot(float xStart, float yStart);

    /**
     * Make this Laser active, and set it's position to `start`
     * @param start vector position to start from
     */
    void shoot(sf::Vector2f);

    /**
     * Get the boundary collider for this laser.
     * For use with collision detection
     *
     * @return sf::FloatRect global bounds rectangle of the laser
     */
    sf::FloatRect getCollider() const;

    /** Change the state of this laser to in-active */
    void deactivate();

    /** @return if this laser is currently active */
    bool isActive() const;

  private:
    // Static properties common to all lasers

    /** Laser speed in px/second. Original game had 7px per frame (60fps). */
    static constexpr float Speed = 7 * 60;

    /** Color of all lasers (Red) */
    static inline const sf::Color Color = sf::Color::Red;

    /** Size of all lasers (px) */
    static inline const sf::Vector2f Size{1.0, 6.0};

    /** Only draw active lasers. */
    bool m_active = false;

    /** Shape of the laser (rectangle) */
    sf::RectangleShape m_shape;
};
