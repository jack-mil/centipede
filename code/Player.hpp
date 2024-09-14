#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:

    // Which directions is the player currently moving in
    bool m_UpPressed;
    bool m_DownPressed;
    bool m_LeftPressed;
    bool m_RightPressed;

    /** The position of the player */
    sf::Vector2f position;

    /** The bounds of player movement */
    sf::IntRect bounds;

    sf::FloatRect size;
    
    /** Player movement speed in pixels/second */
    const float moveSpeed = 600;
public:
    sf::Sprite m_Sprite;
    /** Construct a new Player object */
    Player();

    /** Show the player in the middle of defined player area */
    void spawn(sf::IntRect playerArea);

    /** Do player movement */
    void handleInput();

    /** Update player sprite position based on current input */
    void update(float deltaTime);

    /** Move the player left */
    void moveLeft();

    /** Move the player right */
    void moveRight();

    /** Move the player up */
    void moveUp();

    /** Move the player down */
    void moveDown();

    /** Stop moving left */
    void stopLeft();

    /** Stop moving right */
    void stopRight();

    /** Stop moving up */
    void stopUp();

    /** Stop moving down */
    void stopDown();

    /**
     * Get reference to player Sprite object
     * @return sf::Sprite&
     */
    sf::Sprite &getSprite();
};