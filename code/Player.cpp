#include <iostream>

#include "Player.hpp"
#include "TextureManager.hpp"

/**
 * Constructor initializes the sprite
 * and other members and sets the origin to the center.
 */
Player::Player()
    : m_sprite(TextureManager::GetTexture("graphics/sprites.png"), sf::IntRect(12, 171, 7, 8)),
      m_bounds(), m_size(), m_pos() {

    // use the sprite size to center the origin
    const auto& size = m_sprite.getLocalBounds();
    m_sprite.setOrigin(size.width / 2.f, size.height / 2.f);
}

/**
 * Move the player to the center of the given area,
 * and setup bounds that account for the sprite size.
 * @param playerArea Rectangle that the starship can move in.
 */
void Player::spawn(const sf::FloatRect& playerArea) {

    m_pos.x = playerArea.left + (playerArea.width / 2); // center
    m_pos.y = playerArea.top + playerArea.height;       // bottom row

    m_sprite.setPosition(m_pos);

    const auto& size = m_sprite.getLocalBounds();
    m_bounds.left = playerArea.left + size.width / 2.f;
    m_bounds.top = playerArea.top + size.height / 2.f;

    m_bounds.width = playerArea.width - size.width;
    m_bounds.height = playerArea.height - size.height;
}

/** Set movement flags based on current keyboard input */
void Player::handleInput() {
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
void Player::update(const float deltaTime) {
    // moves `Speed` pixels every second.
    // opposite directions cancel out.
    const float distance = Player::Speed * deltaTime;
    const auto old_pos = m_pos;
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
