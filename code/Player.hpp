#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    /** Player movement speed in pixels/second */
    static inline const float Speed = 600;

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

public:
    /** The position of the player */
    sf::Vector2f m_pos;
    /** Starship player to draw */
    sf::Sprite m_sprite;
    /** Construct a new Player object */
    Player();

    /** Show the player in the middle of defined player area */
    void spawn(const sf::FloatRect& playerArea);

    /** Do player movement */
    void handleInput();

    /** Update player sprite position based on elapsed seconds */
    void update(const float deltaTime);
};