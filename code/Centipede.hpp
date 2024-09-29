/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-20

Description:
The Centipede class controls it's descent down the game area every update.
Multiple Centipedes may be present in the game area with different segment lengths
*/
#pragma once
#include <list>

#include <SFML/Graphics.hpp>

#include "Mushrooms.hpp"
#include "Settings.hpp" // namespace Game

/**
 * Each segment of the centipede acts independently from every other.
 * This allows me to more closely match the movement pattern of the original game.
 *
 * Segments inherit from sf::Sprite, adding extra functionality on top of being drawable.
 */
class Segment : public sf::Sprite
{
  public:
    Segment() = delete; // no default constructor
    Segment(const sf::Texture& tex, const sf::IntRect& rect, sf::FloatRect bounds);

    /** Enum to represent the direction the centipede is moving */
    enum class Moving { Right,
                        Left };
    /** Enum for the states of colliding animation */
    enum class Animation { None,
                           Start,
                           Mid1,
                           Mid2,
                           Final };

    void update(float deltaTime);
    void updateNextState();
    sf::Vector2f getLeftEdge() const;
    sf::Vector2f getRightEdge() const;

    Moving m_direction = Moving::Left;
    Animation m_animation = Animation::None;
    bool m_descending = true;

  private:
    /** Bounding area of centipede movement (px) */
    sf::FloatRect m_bounds;
};


/**
 * A Centipede manages the std::list of sprite Segments.
 * It is the main controller and public interface for the Engine to interact with.
 */
class Centipede
{
  public:
    /** Moves at 15 grid cells per second (2 px/tick) */
    static constexpr float Speed = Game::GridSize * 15;

    /** Collision animation is always 2 px/tick */
    static constexpr float AnimSpeed = 2;

    /** Starting number of Centipede segments */
    static constexpr int MaxLength = 12;

    /** Construct a new Centipede object of max length*/
    Centipede(const sf::FloatRect& bounds);

    /**
     * Construct a new Centipede object
     *
     * @param length Number of segments
     * @param bounds Bounding area for movement
     */
    Centipede(int length, const sf::FloatRect& bounds);

    bool checkMushroomCollision(const std::vector<MushroomManager::Shroom>& shrooms);

    bool checkLaserCollision(sf::FloatRect laser);

    /** Update the centipede position based on elapsed seconds */
    void update(float deltaTime);

    /** Draw all segments to the screen */
    void draw(sf::RenderWindow& target);

  private:
    static inline const sf::IntRect HeadTexOffset{12, 43, 8, 8};   // head texture
    static inline const sf::IntRect BodyTexOffset{116, 251, 8, 8}; // body texture

    void splitAt(std::list<Segment>::iterator segment_it);

    /** The area of movement */
    sf::FloatRect m_bounds;

    // MushroomManager& m_shroomMan;

    /** All of the segments that make up this centipede.
     * The first element is always the head sprite. The other's trail behind. */
    std::list<Segment> m_segments;

};

/**
 * Override the boolean inversion operator to easily switch directions
 * @param dir Moving::Right or Moving::Left
 * @return Opposite direction of `dir`
 */
constexpr Segment::Moving operator!(const Segment::Moving& dir) noexcept;