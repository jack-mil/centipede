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
  private:
    // sf::Shader m_shroomShader;
    /** Area where mushroom can be placed */
    sf::FloatRect m_bounds;
    /** Mersenne twister random number engine */
    std::mt19937 m_rng;

    class Shroom
    {
      public:
        Shroom() = delete;
        Shroom(const sf::Texture& t, const sf::IntRect& r) : sprite{t, r} {};

        sf::Sprite sprite;
        bool active = true;
        int health = 4;
    };

  public:
    /** Construct a empty Mushroom Manager object (no shrooms) */
    MushroomManager() = delete;
    MushroomManager(sf::FloatRect);
    /** Collection of mushroom sprites that this class manages */
    std::vector<Shroom> m_shrooms;
    /** Create the mushroom sprites and randomly distribute
     *  around the game area */
    void spawn();

    void drawAll(sf::RenderWindow& window);

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
};