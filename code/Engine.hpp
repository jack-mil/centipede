#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "Player.hpp"
#include "Laser.hpp"
#include "TextureHolder.hpp"

enum class State { START,
                   PLAYING,
                   GAME_OVER };

class Engine {
private:
    // The texture holder
    TextureHolder th;

    Player m_player;

    // A regular RenderWindow
    sf::RenderWindow m_Window;

    sf::FloatRect m_playerBounds;

    /** Start/Game over screen sprite */
    sf::Sprite m_startSprite;

    /** Game state machine */
    State state = State::START;

	/** Elapsed game time */
	sf::Time m_TotalGameTime;

    /** Time a laser was fired */
    sf::Time m_lastFired;
    /** Firerate of the player */
    const int m_fireRate = 10;

    std::array<Laser, 50> m_lasers;
    int m_currentLaser = 0;
	/** Poll player input and to set state */
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

    // A vector of Vector2f for the fire emiiter locations
    // vector<Vector2f> m_FireEmitters;

public:
    /** Construct a new Engine object */
    Engine();

    /** Create a window and run the entire game loop */
    void run();
};
