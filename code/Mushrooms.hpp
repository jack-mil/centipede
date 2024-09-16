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
class MushroomManager {
private:
    // sf::Texture& m_tex;
    /** Area where mushroom can be placed */
    sf::FloatRect m_bounds;
    /** Mersenne twister random number engine */
    std::mt19937 m_rng;

public:
    /** Construct a empty Mushroom Manager object (no shrooms) */
    MushroomManager();
    /** Create the mushroom sprites and randomly distribute
     *  around the game area */
    void spawn(sf::FloatRect bounds);
    /** Collection of mushroom sprites that this class manages */
    std::vector<sf::Sprite> m_shrooms;
};