/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-16

Description:
Defines the main game Engine and game loop logic.
*/
#include <iostream>

#include "Engine.hpp"
#include "Settings.hpp"

/**
 * Construct a new Engine:: Engine object
 *
 * Initializer list handles creating member objects.
 * Body sets window and view settings
 */
Engine::Engine()
    : texMan(),
      m_window(Game::WindowMode, Game::Name, sf::Style::Close),
      m_view(Game::GameCenter, Game::GameSize),
      m_player(), m_playerBounds(),
      m_shroomMan(Engine::ShroomArea),
      m_centipede{1, Engine::EnemyArea},
      m_spider{Engine::SpiderArea},
      m_lasers(), m_startSprite(TextureManager::GetTexture("graphics/startup-screen-background.png")),
      m_clock(), m_totalGameTime(sf::Time::Zero), m_lastFired(sf::Time::Zero) {

    // set some OS window options
    m_window.setMouseCursorVisible(false);
    m_window.setFramerateLimit(60); // original game was 60fps
    m_window.setVerticalSyncEnabled(false);

    // place the window in the center of the desktop
    m_window.setPosition(sf::Vector2i(
        (sf::VideoMode::getDesktopMode().width / 2) - (m_window.getSize().x / 2),
        (sf::VideoMode::getDesktopMode().height / 2) - (m_window.getSize().y / 2)));

    m_window.setView(m_view);

    // grossly scale the image for now (TODO: need different splash screen)
    m_startSprite.setScale(0.4, 0.5);

    // Calculate the player area (bottom 4 rows)
    m_playerBounds.left = 0.0;
    m_playerBounds.top = m_view.getSize().y - Game::GridSize * 5;
    m_playerBounds.width = m_view.getSize().x;
    m_playerBounds.height = Game::GridSize * 4;
}

/** Main entry-point into the game loop.
 *
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run() {
    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Shaders are not available");
    }
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
            if ((event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) && state == State::Start) {
                // Position the player in bounds
                m_shroomMan.spawn();
                m_player.spawn(m_playerBounds);
                m_spider.spawn();

                state = State::Playing;
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
    if (state == State::Playing) {

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
    if (state != State::Playing) {
        return;
    }
    m_centipede.update(dtSeconds);

    for (auto& laser : m_lasers) {
        // skip updating and colliding with inactive lasers
        if (!laser.active) {
            continue;
        }
        laser.update(dtSeconds); // move the laser upward

        for (auto& shroom : m_shroomMan.m_shrooms) {
            if (!shroom.active) {
                continue;
            }
            if (laser.m_shape.getGlobalBounds().intersects(shroom.sprite.getGlobalBounds())) {
                m_shroomMan.damage(shroom);
                laser.active = false;
            }
        }
        // if (m_centipede.hitBy(laser.m_shape.getGlobalBounds())){
        //     m_centipede.kill();
        // }
    }
    for (const auto& shroom : m_shroomMan.m_shrooms) {
        if (!shroom.active) {
            continue;
        }
        if (m_centipede.getBoundRect().intersects(shroom.sprite.getGlobalBounds())) {
            m_centipede.changeDirection();
        }
    }

    m_player.update(dtSeconds);
    m_spider.update(dtSeconds);
}

/** Draw all game objects to the window.
 *
 * Implements the double buffering sequence of clear-draw-display from SFML.
 */
void Engine::draw() {

    m_window.clear(Engine::WorldColor);

    // draw the start screen at beginning
    if (state == State::Start) {
        m_window.draw(m_startSprite);
    }

    // draw all the objects during game-play
    if (state == State::Playing) {

        auto test = sf::RectangleShape(ShroomArea.getSize());
        test.setFillColor(sf::Color::Transparent);
        test.setOutlineThickness(-2);
        test.setPosition(ShroomArea.getPosition());
        m_window.draw(test);

        auto test2 = sf::RectangleShape(SpiderArea.getSize());
        test2.setFillColor(sf::Color::Transparent);
        test2.setOutlineThickness(-1);
        test2.setOutlineColor(sf::Color::Red);
        test2.setPosition(SpiderArea.getPosition());
        m_window.draw(test2);

        // draw mushrooms
        m_shroomMan.drawAll(m_window);

        m_centipede.draw(m_window);

        // draw lasers
        for (const auto& laser : m_lasers) {
            if (laser.active) {
                m_window.draw(laser.m_shape);
            }
        }

        // draw starship
        m_window.draw(m_player.m_sprite);
        m_window.draw(m_spider.m_sprite);
        // switch to hud overlay sometime...
    }

    m_window.display();
}