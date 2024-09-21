/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-20

Description:
Centipede class definition.
*/

#include "Centipede.hpp"
#include "TextureManager.hpp"

/** Boolean NOT operator overload for easy direction switching */
constexpr Centipede::Moving& operator!(Centipede::Moving& d) noexcept {
    return d = static_cast<Centipede::Moving>(!static_cast<bool>(d));
}

Centipede::Centipede(int length, sf::FloatRect bounds)
    : m_length{length}, m_segments{} {

    // Reserve space for the max number of segments
    m_segments.reserve(Centipede::MaxLength);

    // define texture parameters
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    const sf::IntRect headOffset(44, 251, 8, 8);  // head texture
    const sf::IntRect bodyOffset(116, 251, 8, 8); // body texture
    const auto& size = headOffset.getSize();      // 8x8 px

    // adjust bounds for the sprite size
    // m_bounds.left += size.x / 2.f;
    // m_bounds.top += size.y / 2.f;
    bounds.width -= size.x;
    bounds.height -= size.y;
    // set starting position of the head
    sf::Vector2f startPos{bounds.left + (bounds.width / 2.f), bounds.top};
    // fill the segment vector
    for (size_t i = 0; i < Centipede::MaxLength; i++) {
        // create a new sprite for this segment
        // sf::Sprite segment{tex, i == 0 ? headOffset : bodyOffset};
        Body segment{sf::Sprite{tex, i == 0 ? headOffset : bodyOffset},
                     sf::Vector2f{startPos.x + Game::GridSize * i, startPos.y},
                     bounds};
        // center sprite origin
        // segment.setOrigin(size.x / 2.f, size.y / 2.f);
        // segment.setPosition(m_headPos.x + Game::GridSize * i, m_headPos.y);
        // give ownership to the vector
        m_segments.push_back(std::move(segment));
    }
}

/** Move the segment positions */
void Centipede::update(float deltaTime) {
    for (auto& seg : m_segments) {
        seg.update(deltaTime);
    }
}

/** Draw all segments to the screen */
void Centipede::draw(sf::RenderWindow& target) {
    for (const auto& seg : m_segments) {
        target.draw(seg.m_sprite);
    }
}

/** Return the head bounding rect */
sf::FloatRect Centipede::getBoundRect(){
    return m_segments.front().m_sprite.getGlobalBounds();
}

void Centipede::changeDirection() {
    for (auto &seg : m_segments)
    {
        seg.m_direction = !seg.m_direction;
    }
}

/** Update a single segment independently from the others */
void Centipede::Body::update(float deltaTime) {

    const auto distance = Centipede::Speed * deltaTime;
    switch (m_direction) {
    case Moving::Right:
        m_pos.x += distance;
        break;
    case Moving::Left:
        m_pos.x -= distance;
        break;
    default:
        // shouldn't happen
        break;
    }
    // switch directions and move down if reached the edges
    if (m_pos.x < m_bounds.left || m_pos.x > m_bounds.width + m_bounds.left) {
        m_direction = !m_direction;
        // m_segments[0].rotate(180.0);
        m_pos.y += Game::GridSize;

        if (m_pos.x < m_bounds.left) {
            m_pos.x = m_bounds.left;
        } else {
            m_pos.x = m_bounds.width + m_bounds.left;
        }
    }

    m_sprite.setPosition(m_pos);
}
