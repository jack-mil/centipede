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
      m_window{Game::WindowMode, Game::Name, sf::Style::Close},
      m_view{Game::GameCenter, Game::GameSize},
      m_player{Engine::PlayerArea},
      m_shroomMan{Engine::ShroomArea},
      m_centipede{1, Engine::EnemyArea},
      m_spider{Engine::SpiderArea},
      m_lasers(), m_startSprite{TextureManager::GetTexture("graphics/startup-screen-background.png")},
      m_clock(), m_totalGameTime{sf::Time::Zero}, m_lastFired{sf::Time::Zero}
{

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
}

/** Main entry-point into the game loop.
 *
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run()
{
    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Shaders are not available");
    }
    // reset the clock for first run
    m_clock.restart();
    while (m_window.isOpen()) {
        const sf::Time& dt = m_clock.restart();
        m_totalGameTime += dt;
        m_elapsedTime += dt.asSeconds();
        const float tick = 1 / 60.f;

        // only draw frames at a maximum tick rate
        input();
        if (m_elapsedTime >= tick) {
            update(tick);
            draw();
            m_elapsedTime = 0;
        }
    }
}

/** Handle event input (start/stop/quit),
 * player movement input (from Player::handleInput()),
 * as well as shooting lasers with <SPACE>.
 */
void Engine::input()
{
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
                // Spawn all the enemies
                m_shroomMan.spawn();
                m_player.spawn();
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
                m_lasers[m_currentLaser].shoot(m_player.getGunPosition());
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
void Engine::update(const float dtSeconds)
{
    // only update during the actual game
    if (state != State::Playing) {
        return;
    }

    m_centipede.checkMushroomCollision(m_shroomMan.m_shrooms);

    m_shroomMan.checkSpiderCollision(m_spider.m_sprite.getGlobalBounds());

    m_player.checkSpiderCollision(m_spider.m_sprite.getGlobalBounds());

    for (auto& laser : m_lasers) {
        // skip updating or colliding with inactive lasers
        if (!laser.active) {
            continue;
        }

        if (m_spider.checkLaserCollision(laser.getCollider())) {
            laser.active = false;
            continue;
        }

        if (m_shroomMan.checkLaserCollision(laser.getCollider())) {
            laser.active = false;
            continue;
        }

        if (m_centipede.checkLaserCollision(laser.getCollider())) {
            laser.active = false;
            continue;
        }

        laser.update(dtSeconds); // move the laser upward
    }

    m_centipede.update(dtSeconds);
    m_spider.update(dtSeconds);
    m_player.update(dtSeconds);

    // when the player dies, restart the game
    if (m_player.isDead()) {
        state = State::Start;
    }
}

/** Draw all game objects to the window.
 *
 * Implements the double buffering sequence of clear-draw-display from SFML.
 */
void Engine::draw()
{

    m_window.clear(Engine::WorldColor);

    if (state == State::Start) {
        // draw the start screen at beginning
        m_window.draw(m_startSprite);

    } else if (state == State::Playing) {
        // draw all the objects during game-play

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

        auto test3 = sf::RectangleShape(PlayerArea.getSize());
        test3.setPosition(PlayerArea.getPosition());
        test3.setFillColor(sf::Color::Transparent);
        test3.setOutlineThickness(-0.5);
        test3.setOutlineColor(sf::Color::Green);
        m_window.draw(test3);


        // draw spider
        m_spider.draw(m_window);

        // draw mushrooms
        m_shroomMan.draw(m_window);

        // draw centipede(s)
        m_centipede.draw(m_window);

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