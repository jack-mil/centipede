/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
The spider class moves around the play area clearing mushrooms.
If the player hits the spider, a life is lost and the player is reset.
*/

#pragma once
#include <random>

#include <SFML/Graphics.hpp>

class Spider : public sf::Drawable
{
  public:
    /** Construct a new Spider object that moves within `bounds` */
    Spider(sf::FloatRect bounds);
    // no default constructor
    Spider() = delete;

    void reset();

    /** Set the starting position and state */
    void spawn();

    /** Update the spider's movement based on elapsed time */
    void update(float deltaTime);

    /** Draw only living spiders to the target
     * Sprite overload
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /** Check if a laser hit this spider, and 'kill' it
     * @return true if the spider was hit.
     */
    bool checkLaserCollision(sf::FloatRect collider);

    /**
     * Get the spider collider for collisions
     *
     * @return sf::FloatRect
     */
    sf::FloatRect getCollider() const;

    bool isDead() const;

    /** States for the movement state-machine */
    enum class Moving { Up, Down, UpRight, UpLeft, DownLeft, DownRight };

  private:
    /** The location of the spider texture in the sprite-sheet */
    static inline const sf::IntRect SpiderAnimationOffset[8] =
    {
      {32, 300, 60, 32}, {128, 300, 60, 32}, {224, 300, 60, 32}, {320, 300, 60, 32},
      {32, 364, 60, 32}, {128, 364, 60, 32}, {224, 364, 60, 32}, {320, 364, 60, 32}
    };
    /** Speed of movement in px/s for both x and y components */
    static constexpr float Speed = 240;

    /** Seconds between changing direction */
    const double m_moveDuration = 0.5;

    /** Seconds between changing animation */
    const double m_animationDuration = 0.05;

    /** Seconds to wait before re-spawning */
    const double m_respawnDuration = 5;

    /** The spider sprite */
    sf::Sprite m_sprite;

    /** The area the spider can move in */
    sf::FloatRect m_bounds;

    /** Random number generator for erratic movement */
    std::mt19937 m_rng;

    /** Current direction of movement */
    Moving m_direction;

    int m_animation;

    /** If the spider is alive or note */
    bool m_alive = true;

    // A bunch of properties for controlling the spider movement state-machine
    double m_moveTimer    = 0;
    double m_animationTimer = 0;
    double m_respawnTimer = 0;

    bool m_moveLeft = false;
};