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

Centipede::Centipede(int length, const sf::FloatRect& bounds) : m_length{length}, m_segments{}
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
    // m_bounds.left += size.x / 2.f;
    // m_bounds.top += size.y / 2.f;
    // m_bounds.width -= size.x;
    // m_bounds.height -= size.y;
    // set starting position of the head
    sf::Vector2f startPos{m_bounds.left + (m_bounds.width / 2.f) + 1, m_bounds.top + size.y / 2};
    // fill the segment vector
    for (size_t i = 0; i < Centipede::MaxLength; i++) {
        // create a new sprite for this segment
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

    const float disp = Centipede::Speed * deltaTime;

    // move the segments
    for (auto& seg : m_segments) {

        seg.setNextState();

        // Movement while going straight
        if (seg.animation == Animation::None) {
            switch (seg.m_direction) {
            case Moving::Right:
                seg.move(disp, 0.0);
                break;
            case Moving::Left:
                seg.move(-disp, 0.0);
                break;
            };
        }

        // Movement for the collision animation (descend one row over 4 frames)
        const float yDisp = seg.descending
                                ? Centipede::AnimSpeed
                                : -Centipede::AnimSpeed;
        const float xDisp = seg.m_direction == Moving::Right
                                ? Centipede::AnimSpeed
                                : -Centipede::AnimSpeed;

        switch (seg.animation) { // TODO: set alternate animation intRects here
        case Animation::Start:
            seg.move(xDisp, yDisp);
            seg.animation = Animation::Mid1;
            break;
        case Animation::Mid1:
            seg.move(xDisp, yDisp);
            seg.animation = Animation::Mid2;
            seg.m_direction = seg.m_direction == Moving::Right ? Moving::Left : Moving::Right;
            break;
        case Animation::Mid2:
            seg.move(xDisp, yDisp);
            seg.animation = Animation::Final;
            break;
        case Animation::Final:
            seg.move(xDisp, yDisp);
            seg.animation = Animation::None;
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

bool Centipede::checkMushroomCollission(const std::vector<MushroomManager::Shroom>& shrooms)
{
    const float spacing = 3.0; // 3px from anything is "collision"
    for (auto& seg : m_segments) {

        // Don't check for collisions if currently in a downward animation
        if (seg.animation != Animation::None) {
            continue;
            ;
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
                    seg.animation = Animation::Start;
                }
            } else if (seg.m_direction == Moving::Left) {
                // Check left side of segment with right side of mushroom
                if (segLeft.x - shroomRight.x <= spacing) {
                    seg.animation = Animation::Start;
                }
            }
        }
    }
    return false;
}

void Centipede::Segment::setNextState()
{
    const float spacing = 3.0; // 3px from anything is "collision"
    static const float width = this->getLocalBounds().width;

    const auto centerPos = this->getPosition();
    // Don't check for collisions if currently in a downward animation
    if (animation != Animation::None) {
        return;
    }
    if (m_direction == Moving::Right) {
        if ((m_bounds.left + m_bounds.width) - this->getRightEdge().x <= spacing) {
            animation = Animation::Start;
        }
    } else if (m_direction == Moving::Left) {
        if (this->getLeftEdge().x - m_bounds.left <= spacing) {
            animation = Animation::Start;
        }
    }

    // after descending to the very bottom, bounce around in a 4 row area on the bottom (player area)
    if (descending) {
        // hit bottom edge
        if ((m_bounds.top + m_bounds.height) == (centerPos.y + width / 2)) {
            descending = false;
        }
    } else {
        // ascending, hit top edge
        if ((centerPos.y - width / 2) == m_bounds.top + m_bounds.height - Game::GridSize * 4) {
            descending = true;
        }
    }
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
