#include <iostream>

#include "Player.hpp"
#include "TextureHolder.hpp"

Player::Player() {
    m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/starship.png"));

    size = m_Sprite.getLocalBounds();
    m_Sprite.setOrigin(size.width / 2.f, size.height / 2.f);
}
// void Player::moveLeft() {
//     std::cout << "Moving left" << std::endl;
//     m_LeftPressed = true;
// }

// void Player::moveRight() {
//     std::cout << "Moving right" << std::endl;
//     m_RightPressed = true;
// }

// void Player::moveUp() {
//     std::cout << "Moving up" << std::endl;
//     m_UpPressed = true;
// }

// void Player::moveDown() {
//     std::cout << "Moving down" << std::endl;
//     m_DownPressed = true;
// }

// void Player::stopLeft() {
//     std::cout << "Stopping left" << std::endl;
//     m_LeftPressed = false;
// }

// void Player::stopRight() {
//     std::cout << "Stopping right" << std::endl;
//     m_RightPressed = false;
// }

// void Player::stopUp() {
//     std::cout << "Stopping up" << std::endl;
//     m_UpPressed = false;
// }

// void Player::stopDown() {
//     std::cout << "Stopping down" << std::endl;
//     m_DownPressed = false;
// }

void Player::spawn(sf::IntRect playerArea) {
    position.x = playerArea.width / 2;
    position.y = playerArea.height / 2;

    bounds.left = playerArea.left;
    bounds.width = playerArea.width;
    bounds.top = playerArea.top;
    bounds.height = playerArea.height;
}

void Player::handleInput() {
    m_UpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    m_DownPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    m_LeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    m_RightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}

void Player::update(float deltaTime) {
    if (m_UpPressed) {
        position.y -= moveSpeed * deltaTime;
    }

    if (m_DownPressed) {
        position.y += moveSpeed * deltaTime;
    }

    if (m_RightPressed) {
        position.x += moveSpeed * deltaTime;
    }

    if (m_LeftPressed) {
        position.x -= moveSpeed * deltaTime;
    }

    // prevent movement out of bounds
    if (position.x > bounds.width - size.width) {
        position.x = bounds.width - size.width;
    }
    if (position.x < bounds.left + size.width) {
        position.x = bounds.left + size.width;
    }
    if (position.y > bounds.height - size.height) {
        position.y = bounds.height - size.height;
    }
    if (position.y < bounds.top + size.height) {
        position.y = bounds.top + size.height;
    }

    m_Sprite.setPosition(position);
}
