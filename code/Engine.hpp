/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
Declare the Game Engine. Engine::run() is the main entrypoint into the game.
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "Laser.hpp"
#include "Mushrooms.hpp"
#include "Player.hpp"
#include "TextureManager.hpp"

enum class State {
    START,
    PLAYING,
    GAME_OVER
};
/**
 * The Engine is responsible for:
 *  - setting up the game window,
 *  - main event loop,
 *  - getting user input,
 *  - updating game objects,
 *  - drawing to the frame
 */
class Engine {

    /** Color for the game world background */
    static inline const sf::Color WorldColor = sf::Color(18, 32, 44, 255);

private:
    /** A cache for all textures used on game sprites.
     * default constructor sets up a static pointer to the only instance.
     */
    const TextureManager texMan;

    /** The game RenderWindow */
    sf::RenderWindow m_window;

    /** The player-controlled starship */
    Player m_player;

    /** The area the player can move in */
    sf::FloatRect m_playerBounds;

    /** Manager for all the mushrooms in the scene */
    MushroomManager m_shroomMan;

    /** The area mushrooms spawn in */
    sf::FloatRect m_shroomBounds;

    /** A pool of 30 laser objects to recycle (should be plenty) */
    std::array<Laser, 30> m_lasers;
    int m_currentLaser = 0;

    /** Start/Game over screen sprite */
    sf::Sprite m_startSprite;

    /** Game state machine */
    State state = State::START;

    /** Main game clock */
    sf::Clock m_clock;

    /** Elapsed game time */
    sf::Time m_totalGameTime;

    /** Time a laser was fired */
    sf::Time m_lastFired;

    /** Poll player input and hand-off to objects */
    void input();

    /** Update all game objects in the scene (and detect collisions) */
    void update(const float dtAsSeconds);

    /** Draw all objects the the frame-buffer */
    void draw();

    // Load a new level
    // void loadLevel();

    // Run will call all the private functions
    // bool detectCollisions(PlayableCharacter &character);

    // Make a vector of the best places to emit sounds from
    // void populateEmitters(vector<Vector2f> &vSoundEmitters,
    //   int **arrayLevel);

public:
    /** Construct a new Engine object */
    Engine();
    /** Create a window and run the entire game loop */
    void run();
};
