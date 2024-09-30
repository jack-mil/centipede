/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Spider class definition and implementation
*/

#include <random>

#include "SFML/Graphics.hpp"

#include "Spider.hpp"
#include "TextureManager.hpp"

/** Construction and set up the inherited Sprite properties */
Spider::Spider(sf::FloatRect bounds) : m_rng{std::random_device{}()}
{
    m_sprite.setTexture(TextureManager::GetTexture("graphics/sprites.png"));
    m_sprite.setTextureRect(Spider::SpiderTexOffset);

    const auto& size = m_sprite.getLocalBounds().getSize();
    m_sprite.setOrigin(size.x / 2.f, size.y / 2.f);

    bounds.left += size.x / 2.f;
    bounds.top += size.y / 2.f;
    bounds.width -= size.x;
    bounds.height -= size.y;
    m_bounds = bounds;

    this->spawn();
}

void Spider::spawn()
{
    // start on the top left of it's bounds.
    m_sprite.setPosition(m_bounds.left, m_bounds.top);
    m_direction = Moving::UpRight;
    m_alive     = true;
}

void Spider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_alive)
    {
        target.draw(m_sprite, states);
    }
}

void Spider::update(float deltaTime)
{
    // if currently inactive, increment timer and don't move around
    if (!m_alive)
    {
        m_respawnTimer += deltaTime;
        if (m_respawnTimer >= m_respawnDuration)
        {
            this->spawn();
            m_respawnTimer = 0;
        }
        return;
    }

    float distance = Spider::Speed * deltaTime;
    switch (m_direction)
    {
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

    if (m_sprite.getPosition().x >= m_bounds.left + m_bounds.width)
    {
        m_canMoveLeft = true;
    }

    m_moveTimer += deltaTime;
    // time to pick a new direction?
    if (m_moveTimer >= m_moveDuration)
    {
        // Construct the possible next directions
        std::vector<Moving> allowedDirections;
        if (m_sprite.getPosition().x >= m_bounds.left + m_bounds.width)
        {
            // on right edge
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpLeft, Moving::DownLeft};
        }
        else if (m_sprite.getPosition().x < m_bounds.left)
        {
            // on left edge
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpRight, Moving::DownRight};
        }
        else
        {
            // can move anywhere if not on edges
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpRight, Moving::DownRight};
            if (m_canMoveLeft)
            {
                allowedDirections.assign({Moving::UpLeft, Moving::DownLeft});
            }
        }

        std::uniform_int_distribution<size_t> dist(0, allowedDirections.size() - 1);
        // Pick from a random index in allowed directions
        m_direction = allowedDirections[dist(m_rng)];

        // reset timer and select new random duration
        m_moveTimer = 0;
    }

    // bounce off the edges predictably
    if (m_sprite.getPosition().y < m_bounds.top)
    {
        // On top edge
        switch (m_direction)
        {
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
    }
    else if (m_sprite.getPosition().y >= m_bounds.top + m_bounds.height)
    {
        // On bottom edge
        switch (m_direction)
        {
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

/**  */
bool Spider::checkLaserCollision(sf::FloatRect other)
{
    // only living spiders can be hit
    bool wasHit = m_alive && m_sprite.getGlobalBounds().intersects(other);
    if (wasHit)
    {
        m_alive = false;
    }
    return wasHit;
}

sf::FloatRect Spider::getCollider() const
{
    return m_sprite.getGlobalBounds();
}