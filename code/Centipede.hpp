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
#include "Mushrooms.hpp"

// inline float taxiDistance(const sf::Vector2f& a, const sf::Vector2f& b);
// inline float L1Norm(const sf::Vector2f& v);

class Centipede
{
  public:
    /** Enum to represent the direction the centipede is moving */
    enum class Moving { Right, Left };
    enum class Animation {None, Start, Mid1, Mid2, Final};

    /** Construct a new Centipede object of max length*/
    Centipede(const sf::FloatRect& bounds) : Centipede{Centipede::MaxLength, bounds} {}

    /** Construct a new Centipede object with initial length*/
    Centipede(int length, const sf::FloatRect& bounds);

    bool checkMushroomCollission(const std::vector<MushroomManager::Shroom>& shrooms);

    /** Update the centipede position based on elapsed seconds */
    void update(float deltaTime);

    /** Draw all segments to the screen */
    void draw(sf::RenderWindow& target);

  private:
    /** Moves at 15 grid cells per second (2 px/tick) */
    static constexpr float Speed = Game::GridSize * 15;

    /** Collision animation is always 2 px/tick */
    static constexpr float AnimSpeed = 2;

    static constexpr int MaxLength = 12;

    /** Number of segments in the centipede */
    const int m_length;

    sf::FloatRect m_bounds;

    /** Holds information about each segment of the centipede */
    class Segment : public sf::Sprite
    {
      public:
        Segment(const sf::Texture& tex, const sf::IntRect& rect, sf::FloatRect bounds)
            : sf::Sprite{tex, rect}, m_bounds{bounds} {};

        void update(float deltaTime);
        void setNextState();
        sf::Vector2f getLeftEdge() const;
        sf::Vector2f getRightEdge() const;

        Moving m_direction = Moving::Left;
        Animation animation = Animation::None;
        bool descending = true;

      private:
        /** Bounding area of centipede movement (px) */
        sf::FloatRect m_bounds;
    };

    /** All of the segments that make up this centipede.
     * The first element is always the head sprite. The other's trail behind. */
    std::vector<Segment> m_segments;
};
