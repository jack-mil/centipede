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
// constexpr Centipede::Moving& operator!(Centipede::Moving& d) noexcept {
//     return d = static_cast<Centipede::Moving>(!static_cast<bool>(d));
// }

Centipede::Centipede(int length, const sf::FloatRect& bounds)
    : m_length{length}, m_segments{}
{

    // Reserve space for the max number of segments
    m_segments.reserve(Centipede::MaxLength);

    // define texture parameters
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    const sf::IntRect headOffset(44, 251, 8, 8);  // head texture
    const sf::IntRect bodyOffset(116, 251, 8, 8); // body texture
    const auto& size = headOffset.getSize();      // 8x8 px

    // adjust bounds for the sprite size
    m_bounds = bounds;
    m_bounds.left += size.x / 2.f;
    m_bounds.top += size.y / 2.f;
    m_bounds.width -= size.x;
    m_bounds.height -= size.y;
    // set starting position of the head
    sf::Vector2f startPos{m_bounds.left + (m_bounds.width / 2.f), m_bounds.top};
    // fill the segment vector
    for (size_t i = 0; i < Centipede::MaxLength; i++) {
        // create a new sprite for this segment
        // sf::Sprite segment{tex, i == 0 ? headOffset : bodyOffset};
        Segment segment{tex, i == 0 ? headOffset : bodyOffset, m_bounds};
        // center sprite origin
        segment.setOrigin(size.x / 2.f, size.y / 2.f);
        segment.setPosition(startPos.x + Game::GridSize * i, startPos.y);
        // give ownership to the vector
        m_segments.push_back(std::move(segment));
    }
}

/** Move the segment positions */
void Centipede::update(float deltaTime)
{

    const auto displacement = Centipede::Speed * deltaTime;

    // current head position
    // sf::Vector2f new_pos{m_segments.front().getPosition()};

    // move the segments
    for (auto& seg : m_segments) {
        seg.checkCollisions();
        switch (seg.m_direction) {
        case Moving::Right:
            seg.move(displacement, 0.0);
            // new_pos.x = std::min(new_pos.x + displacement, m_bounds.width + m_bounds.left);
            // if (new_pos.x == m_bounds.width + m_bounds.left) {
            //     m_direction = Moving::Down;
            // }
            break;
        case Moving::Left:
            seg.move(-displacement, 0.0);
            // new_pos.x = std::max(new_pos.x - displacement, m_bounds.left);
            // if (new_pos.x == m_bounds.left) {
            //     m_direction = Moving::Down;
            // }
            break;
        case Moving::DownLeft:
            seg.move(-2, 2);
            seg.m_direction = Moving::DownLeft2;
            // new_pos.y += Game::GridSize;
            // if (new_pos.x == m_bounds.left) {
            //     m_direction = Moving::Right;
            // }
            // if (new_pos.x == m_bounds.width + m_bounds.left) {
            //     m_direction = Moving::Left;
            // }
            // m_segments.front().rotate(180);
        case Moving::DownLeft2:
            seg.move(-3, 2);
            seg.m_direction = Moving::DownLeft3;
            break;
        case Moving::DownLeft3:
            seg.move(2, 2);
            seg.m_direction = Moving::DownLeft4;
            break;
        case Moving::DownLeft4:
            seg.move(2, 2);
            seg.rotate(180);
            seg.m_direction = Moving::Right;
            break;
        case Moving::DownRight:
            seg.move(2, 2);
            seg.m_direction = Moving::DownRight2;
            break;
        case Moving::DownRight2:
            seg.move(3, 2);
            seg.m_direction = Moving::DownRight3;
            break;
        case Moving::DownRight3:
            seg.move(-2, 2);
            seg.m_direction = Moving::DownRight4;
            break;
        case Moving::DownRight4:
            seg.move(-2, 2);
            seg.rotate(180);
            seg.m_direction = Moving::Left;
            break;
        }
    }

    // switch directions and move down if reached the edges
    // if (new_pos.x < m_bounds.left || new_pos.x > m_bounds.width + m_bounds.left) {
    //     // m_direction = !m_direction;
    //     m_direction = Moving::Down;
    //     // m_segments[0].rotate(180.0);

    //     if (new_pos.x < m_bounds.left) {
    //         new_pos.x = m_bounds.left;
    //     } else {
    //         new_pos.x = m_bounds.width + m_bounds.left;
    //     }
    // }

    // m_segments.front().setPosition(new_pos);

    // // Set the sprite positions, make body trail behind
    // // sf::Vector2f old_pos;
    // for (size_t i = 1; i < m_segments.size(); i++) {

    //     const sf::Vector2f& pos = m_segments[i].getPosition();
    //     const sf::Vector2f& otherPos = m_segments[i - 1].getPosition();

    //     sf::Vector2f direction{otherPos - pos};

    //     float seperation = taxiDistance(otherPos, pos);

    //     if (seperation > Game::GridSize) {
    //         m_segments[i].move((direction / L1Norm(direction)) * displacement);
    //     } else {
    //     }
    // }

    // for (auto& seg : m_segments) {

    //     old_pos = seg.getPosition();

    //     seg.setPosition(new_pos);
    //     new_pos = old_pos;
    //     // seg.update(deltaTime);
    // }
}

/** Draw all segments to the screen */
void Centipede::draw(sf::RenderWindow& target)
{
    for (const auto& seg : m_segments) {
        target.draw(seg);
    }
}

/** Return the head bounding rect */
sf::FloatRect Centipede::getBoundRect()
{
    // return m_segments.front().m_sprite.getGlobalBounds();
    return m_segments.front().getGlobalBounds();
}

bool Centipede::Segment::checkCollisions()
{
    const float spacing = 3.0; // 3px from anything is "collision"
    static const float width = this->getLocalBounds().width;

    const auto centerPos = this->getPosition();
    if (m_direction == Moving::Right) {
        if ((m_bounds.left + m_bounds.width) - (centerPos.x + width / 2) <= spacing) {
            m_direction = Moving::DownRight;
            return true;
        }
    } else if (m_direction == Moving::Left) {
        if (m_bounds.left + (centerPos.x-width) <= spacing) {
            m_direction = Moving::DownLeft;
            return true;
        }
    } // specifically don't check collisions when moving down

    // m_direction = !m_direction;
    // auto pos = m_segments.front().getPosition();
    // pos.y += Game::GridSize;
    // m_segments.front().setPosition(pos);
    // // for (auto& seg : m_segments) {
    // //     seg.m_direction = !seg.m_direction;
    // // }
    return false;
}
/**
 * Compute the rectilinear distance between two points.
 * Also known as "taxicab" distance, L1 norm, Manhattan distance
 * @param a point 1
 * @param b point 2
 * @return float sum of distance in x and y
 */
inline float taxiDistance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return L1Norm(a - b);
}

inline float L1Norm(const sf::Vector2f& v)
{
    return std::abs(v.x) + std::abs(v.y);
}

// /** Update a single segment independently from the others */
// void Centipede::Body::update(float deltaTime) {

//     const auto distance = Centipede::Speed * deltaTime;

//     m_sprite.setPosition(m_pos);
// }
