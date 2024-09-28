/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-20

Description:
Centipede class definition.
*/

#include "Centipede.hpp"
#include "Mushrooms.hpp"
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
    const sf::IntRect headOffset(12, 43, 8, 8);     // head texture
    const sf::IntRect bodyOffset(116, 251, 8, 8);   // body texture
    const sf::Vector2i size = headOffset.getSize(); // 8x8 px

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

    const auto disp = Centipede::Speed * deltaTime;

    // move the segments
    for (auto& seg : m_segments) {

        seg.setNextState();

        switch (seg.m_direction) {
        case Moving::Right:
            seg.move(disp, 0.0);
            break;
        case Moving::Left:
            seg.move(-disp, 0.0);
            break;
        };

        switch (seg.m_downward) { // TODO: set alternate animation intRects here
        case Vertical::Down1:
            seg.move(0.0, disp);
            seg.m_downward = Vertical::Down2;
            break;
        case Vertical::Down2:
            seg.move(0.0, disp);
            seg.m_downward = Vertical::Down3;
            seg.m_direction = seg.m_direction == Moving::Right ? Moving::Left : Moving::Right;
            break;
        case Vertical::Down3:
            seg.move(0.0, disp);
            seg.m_downward = Vertical::Down4;
            break;
        case Vertical::Down4:
            seg.move(0.0, disp);
            seg.m_downward = Vertical::None;
            seg.rotate(180);
            break;
        default:
            break;
        }
    }
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

bool Centipede::checkMushroomCollission(const std::vector<MushroomManager::Shroom>& shrooms)
{
    const float spacing = 3.0; // 3px from anything is "collision"
    for (auto& seg : m_segments) {

        // Don't check for collisions if currently in a downward animation
        if (seg.m_downward != Vertical::None) {
            continue;;
        }

        const sf::Vector2f segLeft = seg.getLeftEdge();
        const sf::Vector2f segRight = seg.getRightEdge();

        // Check for collisions with every mushroom on the board
        // TODO: make this more smart?
        for (const auto& shroom : shrooms) {

            // Skip inactive shrooms
            if (!shroom.active) {
                continue;
            }

            const sf::Vector2f shroomLeft = shroom.getLeftEdge();
            const sf::Vector2f shroomRight = shroom.getRightEdge();

            // Skip mushrooms not on the same row (left and right y are the same)
            if (segLeft.y != shroomLeft.y) {
                continue;
            }

            // Detect collisions and transistion segment to down state
            if (seg.m_direction == Moving::Right) {
                // Check right side of segment with left side of mushroom
                if (shroomLeft.x - segRight.x <= spacing) {
                    seg.m_downward = Vertical::Down1;
                }
            } else if (seg.m_direction == Moving::Left) {
                // Check left side of segment with right side of mushroom
                if (segLeft.x - shroomRight.x <= spacing) {
                    seg.m_downward = Vertical::Down1;
                }
            }
        }
    }
    return false;
}

bool Centipede::Segment::setNextState()
{
    const float spacing = 3.0; // 3px from anything is "collision"
    static const float width = this->getLocalBounds().width;

    const auto centerPos = this->getPosition();
    // Don't check for collisions if currently in a downward animation
    if (m_downward != Vertical::None) {
        return false;
    }
    if (m_direction == Moving::Right) {
        if ((m_bounds.left + m_bounds.width) - (centerPos.x + width / 2) <= spacing) {
            m_downward = Vertical::Down1;
            return true;
        }
    } else if (m_direction == Moving::Left) {
        if (m_bounds.left + (centerPos.x - width) <= spacing) {
            m_downward = Vertical::Down1;
            return true;
        }
    }
    return false;
}

sf::Vector2f Centipede::Segment::getRightEdge() const
{
    const float width = this->getLocalBounds().width;
    const sf::Vector2f& center = this->getPosition();
    return sf::Vector2f(center.x + width / 2.0, center.y);
}

sf::Vector2f Centipede::Segment::getLeftEdge() const
{
    const float width = this->getLocalBounds().width;
    const sf::Vector2f& center = this->getPosition();
    return sf::Vector2f(center.x - width / 2.0, center.y);
}

// /**
//  * Compute the rectilinear distance between two points.
//  * Also known as "taxicab" distance, L1 norm, Manhattan distance
//  * @param a point 1
//  * @param b point 2
//  * @return float sum of distance in x and y
//  */
// inline float taxiDistance(const sf::Vector2f& a, const sf::Vector2f& b)
// {
//     return L1Norm(a - b);
// }

// inline float L1Norm(const sf::Vector2f& v)
// {
//     return std::abs(v.x) + std::abs(v.y);
// }

// /** Update a single segment independently from the others */
// void Centipede::Body::update(float deltaTime) {

//     const auto distance = Centipede::Speed * deltaTime;

//     m_sprite.setPosition(m_pos);
// }
