/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Declare the Game Engine. Engine::run() is the main entrypoint into the game.
*/
#pragma once
#include <array>

#include "SFML/Graphics.hpp"

#include "Centipede.hpp"
#include "Laser.hpp"
#include "Mushrooms.hpp"
#include "Player.hpp"
#include "Spider.hpp"
#include "TextureManager.hpp"

/**
 * Used to control the game loop state-machine
 */
enum class State { Start, Playing, GameOver };
/**
 * The Engine is responsible for:
 *  - setting up the game window,
 *  - main event loop,
 *  - getting user input,
 *  - updating game objects,
 *  - drawing to the frame
 */
class Engine
{

  private:
    /** Color for the game world background */
    static inline const sf::Color WorldColor = sf::Color::Black;

    /** The area Centipede can move in */
    static inline const sf::FloatRect EnemyArea{0, Game::GridSize, Game::GameSize.x,
                                                Game::GameSize.y - 2 * Game::GridSize};
    /** The area the spider can move in */
    static inline const sf::FloatRect SpiderArea{0, Game::GridSize * 16, Game::GameSize.x, Game::GridSize * 15};
    /** The area mushrooms spawn in */
    static inline const sf::FloatRect ShroomArea{0, Game::GridSize * 4, Game::GameSize.x, Game::GameSize.y - 48};
    /** The area player can move in (bottom 4 rows) */
    static inline const sf::FloatRect PlayerArea{0, Game::GameSize.y - Game::GridSize * 5, Game::GameSize.x,
                                                 Game::GridSize * 4};

    /** A cache for all textures used on game sprites.
     * default constructor sets up a static pointer to the only instance.
     */
    const TextureManager texMan;

    /** The game RenderWindow */
    sf::RenderWindow m_window;

    /** The game view, always WIDTHxHEIGHT pixels.
     * Much smaller than the OS Window */
    sf::View m_view;

    /** The player-controlled starship */
    Player m_player;

    /** Manager for all the mushrooms in the scene */
    MushroomManager m_shroomMan;

    /** All the centipedes on the screen */
    Centipede m_centipede;

    /** The spider antagonist moves randomly and clears mushrooms */
    Spider m_spider;

    /** A pool of 30 laser objects to recycle (should be plenty) */
    std::array<Laser, 30> m_lasers;
    size_t                m_currentLaser = 0;

    /** Start/Game over screen sprite */
    sf::Sprite m_startSprite;

    /** Visual border for the edge of the game */
    sf::RectangleShape m_border;

    /** Game state machine */
    State state = State::Start;

    /** Main game clock */
    sf::Clock m_clock;

    /** Elapsed game time */
    sf::Time m_totalGameTime;

    /** Elapsed game time */
    double m_elapsedTime = 0;

    /** Time a laser was fired */
    sf::Time m_lastFired;

    /** Poll player input and hand-off to objects */
    void input();

    /** Update all game objects in the scene (and detect collisions) */
    void update(const float dtAsSeconds);

    /** Draw all objects the the frame-buffer */
    void draw();

    /** Resize the viewport to preserve the game aspect ratio when the window is resized
     * @param width, height new size of the main window
     */
    void setViewport(unsigned int width, unsigned int height);

  public:
    /** Construct a new Engine object */
    Engine();
    /** Create a window and run the entire game loop */
    void run();
};
