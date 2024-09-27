/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-20

Description:
Starship player character class definition.
The player can move around a given bounds using the WASD or arrow keys.
If a enemy collides with the player, a life is lost.
*/
#include <iostream>

#include "Player.hpp"
#include "TextureManager.hpp"

/**
 * Constructor initializes the sprite
 * and other members and sets the origin to the center.
 */
Player::Player(sf::FloatRect bounds)
    : m_lives{Player::StartingLives},
      m_sprite{TextureManager::GetTexture("graphics/sprites.png"), sf::IntRect(12, 171, 7, 8)}
{

    // use the sprite size to center the origin
    const auto& size = m_sprite.getLocalBounds();
    m_sprite.setOrigin(size.width / 2.f, size.height / 2.f);

    // adjust the bounds to account for offset sprite center
    bounds.left += size.width / 2.f;
    bounds.top += size.height / 2.f;

    bounds.width -= size.width;
    bounds.height -= size.height;
    m_bounds = bounds;
}

/**
 * Move the player to the center of the given area,
 * and setup bounds that account for the sprite size.
 * @param playerArea Rectangle that the starship can move in.
 */
void Player::spawn()
{
    m_lives = Player::StartingLives;
    this->reset();
}

void Player::reset()
{
    m_pos.x = m_bounds.left + (m_bounds.width / 2); // center
    m_pos.y = m_bounds.top + m_bounds.height;       // bottom row

    m_sprite.setPosition(m_pos);
}

/** Set movement flags based on current keyboard input */
void Player::handleInput()
{
    m_movingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    m_movingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    m_movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    m_movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

/**
 * Move the player position according to movement flags.
 * Prevent going out of bounds.
 * @param deltaTime time in seconds since last update
 */
void Player::update(const float deltaTime)
{
    // moves `Speed` pixels every second.
    // opposite directions cancel out.
    const float distance = Player::Speed * deltaTime;
    if (m_movingUp) {
        m_pos.y -= distance;
    }

    if (m_movingDown) {
        m_pos.y += distance;
    }

    if (m_movingRight) {
        m_pos.x += distance;
    }

    if (m_movingLeft) {
        m_pos.x -= distance;
    }

    // simple inplace saturation check
    auto saturate = [](float v, float lo, float hi) {
        return v < lo   ? lo
               : v > hi ? hi
                        : v;
    };
    // prevent movement out of the player bounding area
    // not using sf::Rec.contains() because of 'sticky' walls issue
    m_pos.x = saturate(m_pos.x, m_bounds.left, m_bounds.width + m_bounds.left);
    m_pos.y = saturate(m_pos.y, m_bounds.top, m_bounds.height + m_bounds.top);

    m_sprite.setPosition(m_pos);
}

/** Detect if hit by the spider and lose a life */
bool Player::checkSpiderCollision(sf::FloatRect spider)
{
    if (m_sprite.getGlobalBounds().intersects(spider)) {
        m_lives--;
        this->reset();
        return true;
    };
    return false;
}

bool Player::checkMushroomCollision(sf::FloatRect shroom)
{
    if (m_sprite.getGlobalBounds().intersects(shroom)) {
        m_colliding = true;
    } else {
        m_colliding = false;
    }
    return m_colliding;
}

bool Player::isDead() const
{
    return m_lives <= 0;
}

/** Calculate the offset from center origin that the top of the laster should start from. */
sf::Vector2f Player::getGunPosition() const
{
    return m_sprite.getPosition() + sf::Vector2f(0.0, m_sprite.getLocalBounds().height / 2.f);
}
