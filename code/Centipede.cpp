/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Centipede class definition.
*/

#include "SFML/Graphics.hpp"
#include <list>

#include "Centipede.hpp"
#include "Mushrooms.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/**
 * Construct a new Centipede object.
 * Builds the list of segment objects and position them.
 */
Centipede::Centipede(const sf::FloatRect& bounds, MushroomManager& shroomMan)
    : m_bounds{bounds}, m_shroomMan{shroomMan}
{

    // set starting position of the head
    const auto& size = HeadTexOffset.getSize(); // 8x8 px
    sf::Vector2f startPos{m_bounds.left + (m_bounds.width / 2.f), m_bounds.top + size.y / 2};

    // construct the sprite segments in-place using  list iterator
    for (size_t i = 0; i < Centipede::MaxLength; i++) {
        auto& new_seg = m_segments.emplace_back(m_bounds);
        new_seg.setPosition(startPos.x + Game::GridSize * i, startPos.y);
    }

    // (Re)set the head texture
    m_segments.front().setHead();
}

/** Move the segment positions */
void Centipede::update(float deltaTime)
{
    // move the segments
    for (auto& seg : m_segments) {
        this->checkMushroomCollision();
        seg.update(deltaTime);
    }
}

/** Draw all segments to the screen */
void Centipede::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& seg : m_segments) {
        target.draw(seg, states);
    }
}

bool Centipede::checkMushroomCollision()
{
    const float spacing = 3.0; // 3px from anything is "collision"
    for (auto& seg : m_segments) {

        // Don't check for collisions if currently in a downward animation
        if (seg.m_animation != Segment::Animation::None) {
            continue;
        }

        const sf::Vector2f segLeft = seg.getLeftEdge();
        const sf::Vector2f segRight = seg.getRightEdge();

        // Check for collisions with every mushroom on the board
        // TODO: make this more smart?
        for (const auto& shroom : m_shroomMan.m_shrooms) {

            // Skip inactive shrooms
            if (!shroom.active) {
                continue;
            }

            const sf::Vector2f& shroomLeft = shroom.getLeftEdge();
            const sf::Vector2f& shroomRight = shroom.getRightEdge();

            // Skip mushrooms not on the same row (left and right y are the same)
            if (segLeft.y != shroomLeft.y) {
                continue;
            }

            // Detect collisions and transistion segment to down state
            if (seg.m_direction == Segment::Moving::Right && shroomLeft.x >= segRight.x) {
                // Check right side of segment with left side of mushroom
                if (shroomLeft.x - segRight.x <= spacing) {
                    seg.m_animation = Segment::Animation::Start;
                }
            } else if (seg.m_direction == Segment::Moving::Left && shroomRight.x <= segLeft.x) {
                // Check left side of segment with right side of mushroom
                if (segLeft.x - shroomRight.x <= spacing) {
                    seg.m_animation = Segment::Animation::Start;
                }
            }
        }
    }
    return false;
}

bool Centipede::checkLaserCollision(sf::FloatRect laser)
{
    // find the first segment that this laser hits
    for (auto seg = m_segments.begin(); seg != m_segments.end(); seg++) {
        if (laser.intersects(seg->getGlobalBounds())) {

            this->splitAt(seg);
            return true;
        }
    }

    // Didn't hit anything
    return false;
}

void Centipede::splitAt(std::list<Segment>::iterator seg_it)
{
    // Add a mushroom at the location of the destroyed segment
    m_shroomMan.addMushroom(seg_it->getPosition());
    // Remove hit sprite from the list, and destroy it
    auto next = m_segments.erase(seg_it);

    // do nothing if we killed a tail segment
    if (next == m_segments.end() || next->isHead()) {
        return;
    }
    // otherwise, the next element becomes a new head
    next->setHead();
    // Force checking the mushrooms now
    this->checkMushroomCollision();
}

/**
 * Segment constructor initializes base sf::Sprite and members.
 * Segments will have a centered origin, unlike SFML Sprites.
 */
Segment::Segment(sf::FloatRect bounds) : m_bounds{bounds}
{
    // All characters on the same sprite-sheet
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");
    this->setTexture(tex);
    this->setTextureRect(Centipede::BodyTexOffset);

    const auto& size = Centipede::BodyTexOffset.getSize();
    this->setOrigin(size.x / 2.f, size.y / 2.f);
}

void Segment::update(float deltaTime)
{
    // Detect collisions with boundary edges,
    // and update the state machine
    this->updateNextState();

    const float distance = Centipede::Speed * deltaTime;

    // Movement while going straight
    if (m_animation == Animation::None) {
        switch (m_direction) {
        case Moving::Right:
            this->move(distance, 0.0);
            break;
        case Moving::Left:
            this->move(-distance, 0.0);
            break;
        };
    }

    // Movement for the collision animation (descend one row over 4 frames)
    const float yDisp = m_descending
                            ? Centipede::AnimSpeed
                            : -Centipede::AnimSpeed;
    const float xDisp = m_direction == Moving::Right
                            ? Centipede::AnimSpeed
                            : -Centipede::AnimSpeed;

    switch (m_animation) { // TODO: set alternate animation intRects here
    case Animation::Start:
        this->move(xDisp, yDisp);
        m_animation = Animation::Mid1;
        break;
    case Animation::Mid1:
        this->move(xDisp, yDisp);
        m_animation = Animation::Mid2;
        m_direction = !m_direction;
        break;
    case Animation::Mid2:
        this->move(xDisp, yDisp);
        m_animation = Animation::Final;
        break;
    case Animation::Final:
        this->move(xDisp, yDisp);
        m_animation = Animation::None;
        this->rotate(180);
        break;
    case Animation::None: // specifically do nothing if
        break;            // not in an animation state
    }
}

void Segment::updateNextState()
{
    const float spacing = 3.0; // 3px from anything is "collision"

    static const float width = this->getLocalBounds().width;
    const auto& centerPos = this->getPosition();

    // Don't check for collisions if currently in a downward animation
    if (m_animation != Animation::None) {
        return;
    }

    if (m_direction == Moving::Right) {
        if ((m_bounds.left + m_bounds.width) - this->getRightEdge().x <= spacing) {
            m_animation = Animation::Start;
        }
    } else if (m_direction == Moving::Left) {
        if (this->getLeftEdge().x - m_bounds.left <= spacing) {
            m_animation = Animation::Start;
        }
    }

    // after descending to the very bottom, bounce around in a 4 row area on the bottom (player area)
    if (m_descending) {
        // hit bottom edge
        if ((m_bounds.top + m_bounds.height) == (centerPos.y + width / 2)) {
            m_descending = false;
        }
    } else {
        // ascending, hit top edge
        if ((centerPos.y - width / 2) == m_bounds.top + m_bounds.height - Game::GridSize * 4) {
            m_descending = true;
        }
    }
}

sf::Vector2f Segment::getRightEdge() const
{
    const float width = this->getLocalBounds().width;
    const sf::Vector2f& center = this->getPosition();
    return sf::Vector2f(center.x + width / 2.0, center.y);
}

sf::Vector2f Segment::getLeftEdge() const
{
    const float width = this->getLocalBounds().width;
    const sf::Vector2f& center = this->getPosition();
    return sf::Vector2f(center.x - width / 2.0, center.y);
}

void Segment::setHead()
{
    this->setTextureRect(Centipede::HeadTexOffset);
    m_isTail = false;
    m_isHead = true;
}

bool Segment::isHead()
{
    return m_isHead;
}

/** Boolean NOT operator overload for easy direction switching */
constexpr Segment::Moving operator!(const Segment::Moving& d) noexcept
{
    return d == Segment::Moving::Right ? Segment::Moving::Left : Segment::Moving::Right;
}
