/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

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
    /**
     * Construct a new Centipede Segment.
     * Initializes the base-class and members.
     * Constructed as normal body segments, set to the head texture with Segment::setHead()
     *
     * @param bounds The bounding area the Centipede can move in (for wall collisions)
     */
    Segment(sf::FloatRect bounds);

    Segment() = delete; // no default constructor

    /** Enum to represent the direction the centipede is moving */
    enum class Moving { Right, Left };
    /** Enum for the states of colliding animation */
    enum class Animation { None, Start, Mid1, Mid2, Final };

    /** Move the segment according to it's current state */
    void update(float deltaTime);

    /** Check for hitting the bound edges, and update state */
    void detectEdgeCollisions();

    /**
     * Check if a segment will collide with a mushroom.
     *
     * Go into collision animation state if it does.
     *
     * @param shroom The mushroom to collide with
     */
    bool detectMushroomCollisions(const Shroom& shroom);

    /**
     * Get the x,y position of the left edge for collisions
     * @return sf::Vector2f
     */
    sf::Vector2f getLeftEdge() const;

    /**
     * Get the x,y position of the right edge for collisions
     * @return sf::Vector2f
     */
    sf::Vector2f getRightEdge() const;

    /** Mark this segment as a centipede head and change the sprite texture */
    void setHead();

    /** Check if this is a centipede head */
    bool isHead();

    /** Check if this segment is currently in a collision animation */
    bool isAnimating();

  private:
    // Texture positions
    static inline const sf::IntRect HeadTexOffset{12, 43, 8, 8};   // head texture
    static inline const sf::IntRect BodyTexOffset{116, 251, 8, 8}; // body texture

    /** The current direction this segment is moving in */
    Moving m_direction = Moving::Left;

    /** The current state of collision. None represents not colliding */
    Animation m_animation = Animation::None;

    /** Bounding area of centipede movement (px) */
    sf::FloatRect m_bounds;

    bool m_descending = true;

    /* Marks a segment as a head type*/
    bool m_isHead = false;
};

/**
 * A Centipede manages the std::list of sprite Segments.
 * It is the main controller and public interface for the Engine to interact with.
 */
class Centipede : public sf::Drawable
{
  public:
    /** Moves at 15 grid cells per second (2 px/tick) */
    static constexpr float Speed = Game::GridSize * 15;

    /** Collision animation is always 2 px/tick */
    static constexpr float AnimSpeed = 2;

    /** Starting number of Centipede segments */
    static constexpr int MaxLength = 12;

    /**
     * Construct a new Centipede object with default length
     *
     * @param shroomMan Reference to MushroomManager
                        for collision and adding new mushrooms (non-owned)
     * @param bounds Bounding area for movement
     */
    Centipede(const sf::FloatRect& bounds, MushroomManager& shroomMan);

    // No copy constructor
    Centipede(const Centipede&) = delete;

    // No copy assignment
    Centipede& operator=(const Centipede&) = delete;

    /** Check all segments against all mushrooms to see if they collide */
    void checkMushroomCollision();

    /** Check if a laser hits any centipede segments*/
    bool checkLaserCollision(sf::FloatRect laser);

    /** Update the centipede position based on elapsed seconds */
    void update(float deltaTime);

    /** Draw all segments to the target window or texture */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  private:
    /**
     * Split the centipede at the given segment, removing it.
     * A mushroom is added at the location of the removed segment.
     * The next segment is changed to the head texture.
     *
     * @param seg_it iterator to the segment that was hit (killed)
     */
    void splitAt(std::list<Segment>::iterator segment_it);

    /** The area of movement */
    sf::FloatRect m_bounds;

    /** Reference to the mushrooms so we can collide and generate more when split.
     * Aggregate member (not owned).
     */
    MushroomManager& m_shroomMan;

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