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
// This is the original Atari game size
#define WIDTH 240
#define HEIGHT 256  // 8 px on the top and bottom are reserved
#define GRID_SIZE 8 // play area is split into a 30x30 grid of 8x8 pixels

#define BOTTOM_HEIGHT HEIGHT / 5.f

/** Default constructor sets up members and creates the game window */
Engine::Engine()
    : texMan(),
      m_window(sf::VideoMode(WIDTH * 4, HEIGHT * 4), "Centipede", sf::Style::Close),
      m_view(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT)),
      m_player(), m_playerBounds(),
      m_shroomMan(), m_shroomBounds(),
      m_lasers(), m_startSprite(),
      m_clock(), m_totalGameTime(sf::Time::Zero), m_lastFired(sf::Time::Zero) {

    // set some OS window options
    m_window.setMouseCursorVisible(false);
    m_window.setFramerateLimit(60); // original game was 60fps
    m_window.setVerticalSyncEnabled(false);
    // place the window in the center of the screen
    m_window.setPosition(sf::Vector2i(
        (sf::VideoMode::getDesktopMode().width / 2) - (m_window.getSize().x / 2),
        (sf::VideoMode::getDesktopMode().height / 2) - (m_window.getSize().y / 2)));

    // view centered so that 0,0 is still top left.
    m_window.setView(m_view);

    // load the sprite texture, and save it's size.
    // m_startSprite.setTexture(TextureManager::GetTexture("graphics/startup-screen-background.png"));
    // const auto& sceneRect = m_startSprite.getLocalBounds();

    m_playerBounds.left = 0.0;
    m_playerBounds.top = m_view.getSize().y - GRID_SIZE * 4;
    m_playerBounds.width = m_view.getSize().x;
    m_playerBounds.height = GRID_SIZE * 4;

    m_shroomBounds.left = 0.0;
    m_shroomBounds.top = GRID_SIZE; // One row reserved for score
    m_shroomBounds.width = m_view.getSize().x;
    m_shroomBounds.height = m_view.getSize().y - 2 * GRID_SIZE;
}

/** Main entry-point into the game loop.
 *
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run() {
    // reset the clock for first run
    m_clock.restart();
    while (m_window.isOpen()) {
        const sf::Time& dt = m_clock.restart();
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

        // preserve the aspect ratio when resizing
        if (event.type == sf::Event::Resized) {
            float windowRatio = static_cast<float>(event.size.width) / event.size.height;
            float viewRatio = m_view.getSize().x / m_view.getSize().y;
            float sizeX = 1;
            float sizeY = 1;
            float posX = 0;
            float posY = 0;

            if (windowRatio > viewRatio) {
                sizeX = viewRatio / windowRatio;
                posX = (1 - sizeX) / 2;
            } else {
                sizeY = windowRatio / viewRatio;
                posY = (1 - sizeY) / 2;
            }

            m_view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
            m_window.setView(m_view);
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