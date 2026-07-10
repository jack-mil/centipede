/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

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
    static constexpr int AnimationFrames = 4;

    /** The location of the spider texture in the sprite-sheet */
    static inline const sf::IntRect SpiderAnimationOffset[AnimationFrames] =
    {
      {16, 16, 60, 32}, {96, 16, 60, 32}, {176, 16, 60, 32}, {256, 16, 60, 32}
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
