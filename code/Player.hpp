/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
Player character declaration.
*/
#pragma once
#include <SFML/Graphics.hpp>

class Player {

private:
    /** Player movement speed in pixels/second */
    static constexpr float Speed = 400;

    /** Up movement key is pressed */
    bool m_movingUp = false;
    /** Down movement key is pressed */
    bool m_movingDown = false;
    /** Left movement key is pressed */
    bool m_movingLeft = false;
    /** Right movement key is pressed */
    bool m_movingRight = false;

    /** The bounds of player movement */
    sf::FloatRect m_bounds;

    /** The size of player sprite */
    sf::FloatRect m_size;

    int m_lives =  3;

public:
    /** The position of the player */
    sf::Vector2f m_pos;
    /** Starship player to draw */
    sf::Sprite m_sprite;
    /** Construct a new Player object */
    Player() = delete;
    Player(sf::FloatRect bounds);

    /** Start the player in the middle of defined player area */
    void spawn();

    /** Do player movement */
    void handleInput();

    /** Update player sprite position based on elapsed seconds */
    void update(float deltaTime);

    /**
     * Check for collisions with spider and decrement life counter
     * @param spider Collider of the spider
     * @return true if hit by spider
     * @return false otherwise
     */
    bool checkSpiderCollision(sf::FloatRect spider);
};