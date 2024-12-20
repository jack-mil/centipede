/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Player character declaration.
*/
#pragma once

#include <SFML/Graphics.hpp>

/**
 * The Player class inherits from SFML Sprite and implements:
 * user input, spider collisions, and keeping track of lives
 */
class Player : public sf::Sprite
{
  public:
    /** Construct a new Player object */
    Player(sf::FloatRect bounds);

    // no default constructor
    Player() = delete;

    /** Start the player in the middle of defined player area */
    void spawn();

    /** Do player movement */
    void handleInput();

    /** Update player sprite position based on elapsed seconds
     * @param deltaTime time in seconds since last update
     */
    void update(float deltaTime);

    /**
     * Check for collisions with spider and decrement life counter
     * @param spider Collider of the spider
     * @return true if hit by spider
     * @return false otherwise
     */
    bool checkSpiderCollision(sf::FloatRect spider);

    /**
     * Check fo collisions with mushrooms, and prevent movement
     * @param shroom mushroom collider
     * @return true if colliding
     * @return false
     */
    bool checkMushroomCollision(sf::FloatRect shroom);

    /**
     * Determine if all the lives are used up.
     *
     * @return true if lives < 0
     * @return false otherwise
     */
    bool isDead() const;

    /**
     * Return the location that the lasers should spawn from.
     *
     * @return sf::Vector2f
     */
    sf::Vector2f getGunPosition() const;

  private:
    /** Player movement speed in pixels/second */
    static constexpr float Speed = 400;

    /** How many lives the player has at start */
    static constexpr int StartingLives = 3;

    /** Location of the player texture in sprite-sheet */
    static inline const sf::IntRect PlayerTexOffset{12, 171, 7, 8};

    /** Move back to the starting position. */
    void reset();

    /** The bounds of player movement */
    sf::FloatRect m_bounds;

    /** Up movement key is pressed */
    bool m_movingUp = false;
    /** Down movement key is pressed */
    bool m_movingDown = false;
    /** Left movement key is pressed */
    bool m_movingLeft = false;
    /** Right movement key is pressed */
    bool m_movingRight = false;

    bool m_colliding = false;

    /** The current lives remaining */
    int m_lives = Player::StartingLives;
};