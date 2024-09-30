/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Starship player character class definition.
The player can move around a given bounds using the WASD or arrow keys.
If a enemy collides with the player, a life is lost.
*/

#include "SFML/Graphics.hpp"

#include "Player.hpp"
#include "TextureManager.hpp"

/** Constructor initializes the Sprite and other members and sets the origin to the center. */
Player::Player(sf::FloatRect bounds)
{
    this->setTexture(TextureManager::GetTexture("graphics/sprites.png"));
    this->setTextureRect(Player::PlayerTexOffset);

    // use the sprite size to center the origin
    const auto& size = this->getLocalBounds();
    this->setOrigin(size.width / 2.f, size.height / 2.f);

    // adjust the bounds to account for offset sprite center
    bounds.left += size.width / 2.f;
    bounds.top += size.height / 2.f;

    bounds.width -= size.width;
    bounds.height -= size.height;
    m_bounds = bounds;

    // move to starting position
    this->spawn();
}

void Player::spawn()
{
    // reset lives and position
    m_lives = Player::StartingLives;
    this->reset();
}

void Player::reset()
{
    // reset position
    sf::Vector2f start{m_bounds.left + (m_bounds.width / 2), // center
                       m_bounds.top + m_bounds.height};      // bottom row
    this->setPosition(start);
}

/** Set movement flags based on current keyboard input */
void Player::handleInput()
{
    // Can be moved with arrows or WASD
    m_movingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    m_movingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    m_movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    m_movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

/**
 * Move the player position according to movement flags.
 * Prevent going out of bounds.
 */
void Player::update(float deltaTime)
{
    // moves `Speed` pixels every second.
    // opposite directions cancel out.
    const float distance = Player::Speed * deltaTime;
    sf::Vector2f pos = this->getPosition();

    if (m_movingUp) {
        pos.y -= distance;
    }

    if (m_movingDown) {
        pos.y += distance;
    }

    if (m_movingRight) {
        pos.x += distance;
    }

    if (m_movingLeft) {
        pos.x -= distance;
    }

    // simple inplace saturation check
    auto saturate = [](float v, float lo, float hi) {
        return v < lo   ? lo
               : v > hi ? hi
                        : v;
    };
    // prevent movement out of the player bounding area
    // not using sf::Rec.contains() because of 'sticky' walls issue
    pos.x = saturate(pos.x, m_bounds.left, m_bounds.width + m_bounds.left);
    pos.y = saturate(pos.y, m_bounds.top, m_bounds.height + m_bounds.top);

    this->setPosition(pos);
}

/** Detect if hit by the spider and lose a life */
bool Player::checkSpiderCollision(sf::FloatRect spider)
{
    if (this->getGlobalBounds().intersects(spider)) {
        m_lives--;
        this->reset();
        return true;
    };
    return false;
}

bool Player::checkMushroomCollision(sf::FloatRect shroom)
{
    if (this->getGlobalBounds().intersects(shroom)) {
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
    return this->getPosition() + sf::Vector2f(0.0, this->getLocalBounds().height / 2.f);
}
