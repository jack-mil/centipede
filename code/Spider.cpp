/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-22

Description:

*/

#include <random>

#include "Spider.hpp"
#include "TextureManager.hpp"

std::normal_distribution<double> Spider::MoveTimeDistibution{Spider::AverageMoveDuration, 1};

Spider::Spider(sf::FloatRect bounds)
    : m_sprite{TextureManager::GetTexture("graphics/sprites.png"), sf::IntRect{8, 75, 15, 8}},
      m_rng{std::random_device{}()}
{
    const auto& size = m_sprite.getLocalBounds();
    m_sprite.setOrigin(size.width / 2.f, size.height / 2.f);

    bounds.left += size.width / 2.f;
    bounds.top += size.height / 2.f;
    bounds.width -= size.width;
    bounds.height -= size.height;
    m_bounds = bounds;
}

void Spider::spawn()
{
    // start on the top left of it's bounds.
    m_sprite.setPosition(m_bounds.left, m_bounds.top);
    m_direction = Moving::UpRight;
    // m_moveDuration = MoveTimeDistibution(m_rng);
    m_moveDuration = 0.5;
}

void Spider::update(float deltaTime)
{
    float distance = Spider::Speed * deltaTime;
    switch (m_direction) {
    case Moving::Up:
        m_sprite.move(0.0, -distance);
        break;
    case Moving::Down:
        m_sprite.move(0.0, distance);
        break;
    case Moving::DownLeft:
        m_sprite.move(-distance, distance);
        break;
    case Moving::DownRight:
        m_sprite.move(distance, distance);
        break;
    case Moving::UpLeft:
        m_sprite.move(-distance, -distance);
        break;
    case Moving::UpRight:
        m_sprite.move(distance, -distance);
        break;
    }

    if (m_sprite.getPosition().x >= m_bounds.left + m_bounds.width) {
        m_canMoveLeft = true;
    }

    m_moveTimer += deltaTime;
    if (m_moveTimer >= m_moveDuration) {
        // Construct the possible next directions
        std::vector<Moving> allowedDirections;
        if (m_sprite.getPosition().x >= m_bounds.left + m_bounds.width) {
            // on left edge
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpLeft, Moving::DownLeft};
        } else if (m_sprite.getPosition().x < m_bounds.left) {
            // on right edge
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpRight, Moving::DownRight};
        } else {
            // can move anywhere if not on edges
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpRight, Moving::DownRight, Moving::UpLeft, Moving::DownLeft};
        }

        std::uniform_int_distribution<int> dist(0, allowedDirections.size() - 1);
        // Pick from a random index in allowed directions
        m_direction = allowedDirections[dist(m_rng)];

        // reset timer and select new random duration
        m_moveTimer = 0;
        // m_moveDuration = MoveTimeDistibution(m_rng);
    }

    if (m_sprite.getPosition().y < m_bounds.top) {
        // On top edge
        switch (m_direction) {
        case Moving::UpLeft:
            m_direction = Moving::DownLeft;
            break;
        case Moving::Up:
            m_direction = Moving::Down;
            break;
        case Moving::UpRight:
            m_direction = Moving::DownRight;
            break;
        default:
            break;
        }
    } else if (m_sprite.getPosition().y >= m_bounds.top + m_bounds.height) {
        // On bottom edge
        switch (m_direction) {
        case Moving::Down:
            m_direction = Moving::Up;
            break;
        case Moving::DownRight:
            m_direction = Moving::UpRight;
            break;
        case Moving::DownLeft:
            m_direction = Moving::UpLeft;
            break;
        default:
            break;
        }
    }
}