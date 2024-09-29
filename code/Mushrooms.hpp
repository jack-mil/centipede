/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
The MushroomManager class declaration.
*/
#pragma once
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

/**
 * MushroomManager is used to operate on all mushroom sprites in the scene.
 * Each mushroom starts with the same texture.
 */
class MushroomManager
{

  public:
    /** Construct a empty Mushroom Manager object (no shrooms) */
    MushroomManager() = delete;
    MushroomManager(sf::FloatRect);

    class Shroom
    {
      public:
        Shroom() = delete;
        Shroom(const sf::Texture& t, const sf::IntRect& r) : sprite{t, r} {};

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

        sf::Sprite sprite;
        bool active = true;
        int health = 4;
    };

    /** Collection of mushroom sprites that this class manages */
    std::vector<Shroom> m_shrooms;
    /** Create the mushroom sprites and randomly distribute
     *  around the game area */
    void spawn();

    /**
     * Draw all active mushrooms to the scene
     *
     * @param target Window or Texture to draw to
     */
    void draw(sf::RenderTarget& window);

    /**
     * Add a new mushroom to the collection
     *
     * @param location coordinate of top-left corner
     */
    void addMushroom(sf::Vector2f location);

    /** Change the sprite to the damaged version */
    static void damage(Shroom& shroom);

    /**
     * Checks for a spider colliding with any mushroom.
     * Immediately removes the mushroom if hit
     *
     * @param spider The spider collider
     * @return true if the spider hit a mushroom
     * @return false otherwise
     */
    bool checkSpiderCollision(sf::FloatRect other);

    /**
     * Checks for a laser colliding with any mushroom.
     *
     * @param laser The laser collider
     * @return true if a mushroom was hit
     * @return false if nothing was hit
     */
    bool checkLaserCollision(sf::FloatRect laser);

  private:
    // Constant regions for mushroom textures in the sprite-sheet
    static inline const sf::IntRect FullTexOffset{104, 107, 8, 8};
    static inline const sf::IntRect Damage1TexOffset{152, 107, 8, 8};
    static inline const sf::IntRect Damage2TexOffset{136, 107, 8, 8};
    static inline const sf::IntRect Damage3TexOffset{120, 107, 8, 8};

    /** Area where mushroom can be placed */
    sf::FloatRect m_bounds;

    /** Mersenne twister random number engine (for random positioning) */
    std::mt19937 m_rng;
};