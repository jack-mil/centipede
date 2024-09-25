/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-22

Description:

*/

#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Spider
{
  public:
    /** Speed of movement in px/s for both x and y components */
    static constexpr float Speed = 60;
    /**Move for 1 second before changing directions on average */
    static constexpr float AverageMoveDuration = 0.5;

    /** No default constructor */
    Spider() = delete;

    /** Construct a new Spider object */
    Spider(sf::FloatRect bounds);

    void spawn();

    void die();

    void update(float deltaTime);

    void draw(sf::RenderTarget& target);

    bool checkLaserCollision(sf::FloatRect collider);

    sf::Sprite m_sprite;

    /** States for the movement state-machine */
    enum class Moving { Up,
                        Down,
                        UpRight,
                        UpLeft,
                        DownLeft,
                        DownRight };

  private:
    // static std::normal_distribution<double> MoveTimeDistibution;

    /** Current direction of movement */
    Moving m_direction;
    double m_moveTimer = 0;
    double m_respawnTimer = 0;
    /** Seconds between changing direction */
    const double m_moveDuration = 0.5;
    /** Seconds to wait before respawning */
    const double m_respawnDuration = 5;
    bool m_canMoveLeft = false;
    bool m_hitByLaser = false;
    bool m_alive = true;
    sf::FloatRect m_bounds;

    std::mt19937 m_rng;
};