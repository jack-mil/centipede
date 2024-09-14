#include <iostream>

#include "Player.hpp"
#include "TextureHolder.hpp"

Player::Player() {
    m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/starship.png"));

    m_size = m_Sprite.getLocalBounds();
    m_Sprite.setOrigin(m_size.width / 2.f, m_size.height / 2.f);
}

void Player::spawn(const sf::FloatRect& playerArea) {
    m_pos.x = playerArea.left + (playerArea.width / 2);
    m_pos.y = playerArea.top + (playerArea.height / 2);

    m_bounds.left = playerArea.left + m_size.width / 2.f;
    m_bounds.top = playerArea.top + m_size.height / 2.f;

    m_bounds.width = playerArea.width - m_size.width;
    m_bounds.height = playerArea.height - m_size.height;
}

void Player::handleInput() {
    m_UpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    m_DownPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    m_LeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    m_RightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}

/**
 * Move the player position according to movement direction.
 * Prevent going out of set bounds.
 * @param deltaTime time elapsed since last update in seconds
 */
void Player::update(const float deltaTime) {
    // moves `m_speed` pixels every second.
    // opposite directions cancel out.
    const float distance = m_speed * deltaTime;
    const auto old_pos = m_pos;
    if (m_UpPressed) {
        m_pos.y -= distance;
    }

    if (m_DownPressed) {
        m_pos.y += distance;
    }

    if (m_RightPressed) {
        m_pos.x += distance;
    }

    if (m_LeftPressed) {
        m_pos.x -= distance;
    }

    // simple inplace saturation check
    auto saturate = [](float v, float lo, float hi) {
        return v < lo   ? lo
               : v > hi ? hi
                        : v;
    };
    // prevent movement out of the player bounding area.
    // this method prevents "sticking" to the walls when using sf::Rec.contains()
    m_pos.x = saturate(m_pos.x, m_bounds.left, m_bounds.width + m_bounds.left);
    m_pos.y = saturate(m_pos.y, m_bounds.top, m_bounds.height + m_bounds.top);

    m_Sprite.setPosition(m_pos);
}
