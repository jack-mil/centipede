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
    sf::Vector2f m_pos;

    /** The bounds of player movement */
    sf::FloatRect m_bounds;

    /** The size of player sprite */
    sf::FloatRect m_size;

    /** Player movement speed in pixels/second */
    const float m_speed = 600;

public:
    sf::Sprite m_Sprite;
    /** Construct a new Player object */
    Player();

    /** Show the player in the middle of defined player area */
    void spawn(const sf::FloatRect& playerArea);

    /** Do player movement */
    void handleInput();

    /** Update player sprite position based on elapsed seconds */
    void update(const float deltaTime);

};