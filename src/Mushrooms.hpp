/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
The MushroomManager and Shroom class declaration.
It creates a collection of Shroom objects, and handles their state throughout the game.
*/
#pragma once
#include <list>
#include <random>

#include <SFML/Graphics.hpp>

class Shroom : public sf::Sprite
{
  public:
    /**
     * Construct a new Shroom centered at postion (x,y)
     * @param x coordinate of center
     * @param y coordinate of center
     */
    Shroom(float x, float y);
    Shroom(sf::Vector2f location);
    Shroom() = delete; // no default constructor

    /** Decrement the health of this mushroom, and cycle through the textures.
     *  @return remaining health
     */
    int damage();

    /**
     * Get the x,y position of the left side in world space.
     * The y position is simply the center of the mushroom sprite.
     * @return sf::Vector2f (x,y)
     */
    sf::Vector2f getLeftEdge() const;

    /**
     * Get the x,y position of the right side in world space.
     * The y position is simply the center of the mushroom sprite.
     * @return sf::Vector2f (x,y)
     */
    sf::Vector2f getRightEdge() const;

  private:
    // Constant regions for mushroom textures in the sprite-sheet
    static inline const sf::IntRect FullTexOffset{104, 107, 8, 8};
    static inline const sf::IntRect Damage1TexOffset{152, 107, 8, 8};
    static inline const sf::IntRect Damage2TexOffset{136, 107, 8, 8};
    static inline const sf::IntRect Damage3TexOffset{120, 107, 8, 8};

    /** The health of mushroom (starts at 4) */
    int m_health = 4;
};

/**
 * MushroomManager is used to operate on all mushroom sprites in the scene.
 * Each mushroom starts with the same texture.
 */
class MushroomManager : public sf::Drawable
{
  public:
      /** Construct the Mushroom Manager object
     * and create a bunch of mushrooms with random positions
     */
    MushroomManager(sf::FloatRect bounds);
    MushroomManager() = delete; // no default constructor

    /**
     * Draw all active mushrooms to the scene
     * Implements sf::Drawable.draw
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * Add a new mushroom to the collection
     *
     * @param location coordinate of top-left corner
     */
    void addMushroom(sf::Vector2f location);

    /**
     * Checks for a spider colliding with any mushroom.
     * Immediately removes the mushroom if hit
     *
     * @param spider The spider collider
     * @return true if the spider hit a mushroom
     */
    bool checkSpiderCollision(sf::FloatRect other);

    /**
     * Checks for a laser colliding with any mushroom.
     *
     * @param laser The laser collider
     * @return true if the laser hit a mushroom
     */
    bool checkLaserCollision(sf::FloatRect laser);

    /**
     * Get a reference to the list of mushroom sprites for easy iteration
     * @return read-only reference to the internal list
     */
    const std::list<Shroom>& getShrooms() const;

  private:
    /** Collection of mushroom sprites that this class manages */
    std::list<Shroom> m_shrooms;

    /** Area where mushroom can be placed */
    sf::FloatRect m_bounds;

    /** Mersenne twister random number engine (for random positioning) */
    std::mt19937 m_rng;
};