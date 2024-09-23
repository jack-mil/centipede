/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-22

Description:

*/

#pragma once
#include <random>
#include <SFML/Graphics.hpp>

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

    void update(const float deltaTime);

    sf::Sprite m_sprite;

    /** States for the movement state-machine */
    enum class Moving { Up, Down, UpRight, UpLeft, DownLeft, DownRight };

  private:
    static std::normal_distribution<double> MoveTimeDistibution;

    /** Current direction of movement */
    Moving m_direction;
    double m_moveTimer = 0;
    double m_moveDuration;
    bool m_canMoveLeft = false;
    sf::FloatRect m_bounds;

    std::mt19937 m_rng;

};