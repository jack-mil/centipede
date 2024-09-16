/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
Defines the main game Engine and game loop logic.
*/
#include "Engine.hpp"
#include <iostream>

// Window size constants
// Strange size to match given splash image
#define WIDTH 1036
#define HEIGHT 569

#define BOTTOM_HEIGHT HEIGHT / 5.f

/** Default constructor sets up members and creates the game window */
Engine::Engine()
    : texMan(),
      m_window(sf::VideoMode(WIDTH, HEIGHT), "Centipede", sf::Style::Close),
      m_player(), m_playerBounds(),
      m_shroomMan(), m_shroomBounds(),
      m_lasers(), m_startSprite(),
      m_clock(), m_totalGameTime(sf::Time::Zero), m_lastFired(sf::Time::Zero) {

    // set some OS window options
    m_window.setMouseCursorVisible(false);
    m_window.setFramerateLimit(60); // original game was 60fps
    m_window.setVerticalSyncEnabled(false);

    // place the window in the center of the screen
    const auto& desktop = sf::VideoMode::getDesktopMode();
    const auto& winSize = m_window.getSize();
    m_window.setPosition(sf::Vector2i((desktop.width / 2) - (winSize.x / 2),
                                      (desktop.height / 2) - (winSize.y / 2)));

    // load the sprite texture, and save it's size.
    m_startSprite.setTexture(TextureManager::GetTexture("graphics/startup-screen-background.png"));
    const auto& sceneRect = m_startSprite.getLocalBounds();

    m_playerBounds.left = sceneRect.left;
    m_playerBounds.top = sceneRect.height - sceneRect.height / 5.f;
    m_playerBounds.width = sceneRect.width;
    m_playerBounds.height = sceneRect.height / 5.f;

    m_shroomBounds.left = sceneRect.left;
    m_shroomBounds.top = sceneRect.top + 60; // 60px reserved at the top for info
    m_shroomBounds.width = sceneRect.width;
    m_shroomBounds.height = sceneRect.height - 60 - m_playerBounds.height;
}

/** Main entry-point into the game loop.
 *
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run() {
    while (m_window.isOpen()) {
        const sf::Time dt = m_clock.restart();
        m_totalGameTime += dt;
        const float dtSeconds = dt.asSeconds();

        input();
        update(dtSeconds);
        draw();
    }
}

/** Handle event input (start/stop/quit),
 * player movement input (from Player::handleInput()),
 * as well as shooting lasers with <SPACE>.
 */
void Engine::input() {
    // handle event polling for some inputs (start/end, etc)
    sf::Event event;
    while (m_window.pollEvent(event)) {

        // Close the window when "X" button clicked
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed) {

            // Start game from "menu" with "ENTER"
            if ((event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) && state == State::START) {
                // Position the player in bounds
                m_shroomMan.spawn(m_shroomBounds);
                m_player.spawn(m_playerBounds);

                state = State::PLAYING;
                std::cout << "Started" << std::endl;
                m_clock.restart(); // restart clock to prevent frame skip
            }

            // Quit game whenever "ESC" pressed
            if (event.key.code == sf::Keyboard::Escape) {
                std::cout << "Ended" << std::endl;
                m_window.close();
            }
        }
    } // end event polling

    // Keyboard polling for smooth player movement
    if (state == State::PLAYING) {

        // Handle player movement with WASD keys
        m_player.handleInput();

        // Handle shooting lasers
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

            if (m_totalGameTime.asMilliseconds() - m_lastFired.asMilliseconds() > 1000 / Laser::fire_speed) {
                m_lasers[m_currentLaser].shoot(m_player.m_pos);
                m_currentLaser++;
                if (m_currentLaser > m_lasers.size()) {
                    m_currentLaser = 0;
                }
                m_lastFired = m_totalGameTime;
            }
        }
    } // end input while playing
}

/**
 * Update all object positions and state.
 *
 * @param dtSeconds time since last frame
 */
void Engine::update(const float dtSeconds) {
    if (state == State::PLAYING) {

        for (auto& laser : m_lasers) {
            if (laser.active) {
                laser.update(dtSeconds);
            }
        }

        m_player.update(dtSeconds);
    }
}

/** Draw all game objects to the window.
 *
 * Implements the double buffering sequence of clear-draw-display from SFML.
 */
void Engine::draw() {

    m_window.clear(Engine::WorldColor);

    // draw the start screen at beginning
    if (state == State::START) {
        m_window.draw(m_startSprite);
    }

    // draw all the objects during game-play
    if (state == State::PLAYING) {

        auto test = sf::RectangleShape(m_shroomBounds.getSize());
        test.setPosition(m_shroomBounds.getPosition());
        m_window.draw(test);

        // draw mushrooms
        for (const auto& shroom : m_shroomMan.m_shrooms) {
            m_window.draw(shroom);
        }

        // draw lasers
        for (const auto& laser : m_lasers) {
            if (laser.active) {
                m_window.draw(laser.m_shape);
            }
        }

        // draw starship
        m_window.draw(m_player.m_sprite);

        // switch to hud overlay sometime...
    }

    m_window.display();
}