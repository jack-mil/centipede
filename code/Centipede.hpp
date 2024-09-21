/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-20

Description:
The Centipede class controls it's descent down the game area every update.
Multiple Centipedes may be present in the game area with different segment lengths
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Settings.hpp" // namespace Game

class Centipede {
  public:
    /** Enum to represent the direction the centipede is moving */
    enum class Moving : bool { Right,
                               Left };

    /** Construct a new Centipede object */
    Centipede(sf::FloatRect& bounds) : Centipede{Centipede::MaxLength, bounds} {}

    /** Construct a new Centipede object with initial length*/
    Centipede(int length, sf::FloatRect bounds);

    /** Get the global bounds to see if anything intersects with the centipede */
    sf::FloatRect getBoundRect();

    void changeDirection();

    /** Update the centipede position based on elapsed seconds */
    void update(float deltaTime);

    /** Draw all segments to the screen */
    void draw(sf::RenderWindow& target);

  private:
    /** Moves at 7.5 grid cells per second */
    static constexpr float Speed = Game::GridSize * 8;

    static constexpr int MaxLength = 12;

    /** Number of segments in the centipede */
    const int m_length;

    /** Direction the centipede is moving */
    Moving m_direction = Moving::Left;

    /** Holds information about each segment of the centipede */
    class Body {
      public:
        Body() = delete; // no default constructor
        Body(sf::Sprite sprite, sf::Vector2f pos, sf::FloatRect bounds)
            : m_sprite{sprite}, m_pos{pos}, m_bounds{bounds} {};
        sf::Sprite m_sprite;

        void update(float deltaTime);

        Moving m_direction = Moving::Left;
      private:

        sf::Vector2f m_pos;
        /** Bounding area of centipede movement (px) */
        sf::FloatRect m_bounds;
    };

    /** All of the segments that make up this centipede.
     * The first element is always the head sprite. The other's trail behind. */
    std::vector<Body> m_segments;
};
