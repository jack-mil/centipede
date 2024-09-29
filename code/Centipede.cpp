/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-20

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
 * Delegate to main constructor using default length
 */
Centipede::Centipede(const sf::FloatRect& bounds)
    : Centipede{Centipede::MaxLength, bounds}
{
}

/**
 * Construct a new Centipede object.
 * Builds the list of segment objects and position them. */
Centipede::Centipede(int length, const sf::FloatRect& bounds)
    : m_bounds{bounds}, m_segments{}
{

    // All characters on the same sprite-sheet
    const auto& tex = TextureManager::GetTexture("graphics/sprites.png");

    const sf::Vector2i size = HeadTexOffset.getSize(); // 8x8 px

    // set starting position of the head
    sf::Vector2f startPos{m_bounds.left + (m_bounds.width / 2.f) + 1, m_bounds.top + size.y / 2};

    // construct the sprite segments in-place using  list iterators
    for (size_t i = 0; i < Centipede::MaxLength; i++) {
        auto& new_seg = m_segments.emplace_back(tex, BodyTexOffset, m_bounds);
        new_seg.setOrigin(size.x / 2.f, size.y / 2.f);
        new_seg.setPosition(startPos.x + Game::GridSize * i, startPos.y);
    }

    // (Re)set the head texture
    m_segments.front().setTextureRect(HeadTexOffset);
}

/** Move the segment positions */
void Centipede::update(float deltaTime)
{
    // move the segments
    for (auto& seg : m_segments) {

        seg.update(deltaTime);
    }
}

/** Draw all segments to the screen */
void Centipede::draw(sf::RenderWindow& target)
{
    for (const auto& seg : m_segments) {
        target.draw(seg);
    }
}

bool Centipede::checkMushroomCollision(const std::vector<MushroomManager::Shroom>& shrooms)
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
            if (seg.m_direction == Segment::Moving::Right) {
                // Check right side of segment with left side of mushroom
                if (shroomLeft.x - segRight.x <= spacing) {
                    seg.m_animation = Segment::Animation::Start;
                }
            } else if (seg.m_direction == Segment::Moving::Left) {
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

/**
 * When hit by a laser, remove the hit segment (TODO: replace with mushroom)
 * Set a new head, and reverse the direction of back end
 * @param seg_it iterator to the segment that was hit (killed)
 */
void Centipede::splitAt(std::list<Segment>::iterator seg_it)
{
    auto new_head = m_segments.erase(seg_it); // remove sprite from the list, and destroy it
    new_head->setTextureRect(Centipede::HeadTexOffset);

    while (new_head != m_segments.end()) {
        new_head->m_direction = !new_head->m_direction;
        new_head->rotate(180);
        new_head++;
    }
}

/**
 * Construct a new Centipede Segment.
 * Initializes the base-class and members.
 *
 * @param tex The texture to use for this sprite
 * @param rect The region of the texture to use
 * @param bounds The bounding area the Centipede can move in (for wall collisions)
 */
Segment::Segment(const sf::Texture& tex, const sf::IntRect& rect, sf::FloatRect bounds)
    : sf::Sprite{tex, rect}, m_bounds{bounds}
{
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
    const auto centerPos = this->getPosition();

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

/** Boolean NOT operator overload for easy direction switching */
constexpr Segment::Moving operator!(const Segment::Moving& d) noexcept
{
    return d == Segment::Moving::Right ? Segment::Moving::Left : Segment::Moving::Right;
}
