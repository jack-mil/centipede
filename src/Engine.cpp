/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Defines the main game Engine and game loop logic.
*/
#include <iostream>

#include <SFML/Graphics.hpp>

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
      m_view{Game::GameCenter, Game::GameSize},
      m_player{Game::PlayerArea},
      m_shroomMan{Game::ShroomArea},
      m_centipede{Game::EnemyArea, m_shroomMan},
      m_spider{Game::SpiderArea},
      m_totalGameTime{sf::Time::Zero},
      m_lastFired{sf::Time::Zero}
{

    // calculate the window size to be 3/4 of available height

    const auto& desktop   = sf::VideoMode::getDesktopMode();
    const uint  maxHeight = 3 * (desktop.height / 4);

    const float gameRatio = Game::GameSize.x / Game::GameSize.y;
    const uint  maxWidth  = static_cast<uint>(gameRatio * static_cast<float>(maxHeight));

    sf::VideoMode windowSize{maxWidth, maxHeight};

    // (re)create the window (allow resizing)
    m_window.create(windowSize, Game::Name, sf::Style::Default);

    // set some OS window options
    m_window.setMouseCursorVisible(false);
    m_window.setFramerateLimit(60); // original game was 60fps
    m_window.setVerticalSyncEnabled(false);

    // place the window in the center of the desktop
    const auto xpos = (desktop.width / 2u) - (m_window.getSize().x / 2u);
    const auto ypos = (desktop.height / 2u) - (m_window.getSize().y / 2u);
    m_window.setPosition(sf::Vector2i(static_cast<int>(xpos), static_cast<int>(ypos)));

    m_window.setView(m_view);

    // made my own startup image
    m_startSprite.setTexture(TextureManager::GetTexture("graphics/splash.png"));

    // later as a HUD overlay (not submitted)
    // m_border.setSize(Game::GameSize);
    // m_border.setOutlineThickness(-2);
    // m_border.setFillColor(sf::Color::Transparent);
    // m_border.setOutlineColor(sf::Color::White);
}

/**
 * Main entry-point into the game loop.
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run()
{
    if (!sf::Shader::isAvailable())
    {
        throw std::runtime_error("Shaders are not available");
    }
    // reset the clock for first run
    m_clock.restart();
    while (m_window.isOpen())
    {
        const sf::Time& dt = m_clock.restart();
        m_totalGameTime += dt;
        m_elapsedTime += dt.asSeconds();
        constexpr float tick = 1 / 60.f;

        // only draw frames at a maximum tick rate
        input();
        if (m_elapsedTime >= tick)
        {
            update(tick);
            draw();
            m_elapsedTime = 0;
        }
    }
}

/**
 * Handle event input (start/stop/quit),
 * player movement input (from Player::handleInput()),
 * as well as shooting lasers with <SPACE>.
 */
void Engine::input()
{
    // handle event polling for some inputs (start/end, etc)
    sf::Event event;
    while (m_window.pollEvent(event))
    {

        // Close the window when "X" button clicked
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        // preserve the aspect ratio when resizing
        if (event.type == sf::Event::Resized)
        {
            this->setViewport(event.size.width, event.size.height);
        }

        if (event.type == sf::Event::KeyPressed)
        {

            // Start game from "menu" with "ENTER"
            if (state == State::Start && (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space))
            {

                state = State::Playing;
                std::cout << "Started" << std::endl;
                m_clock.restart(); // restart clock to prevent frame skip

                m_player.spawn(); // respawn the player if they are dead
            }

            // Quit game whenever "ESC" pressed
            if (event.key.code == sf::Keyboard::Escape)
            {
                std::cout << "Ended" << std::endl;
                m_window.close();
            }
        }
    } // end event polling

    // Keyboard polling for smooth player movement
    if (state == State::Playing)
    {

        // Handle player movement with WASD keys
        m_player.handleInput();

        // Handle shooting lasers (TODO: move to Player (?) probably)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            auto elapsed = m_totalGameTime.asMilliseconds() - m_lastFired.asMilliseconds();

            // only fire after the firing period has elapsed
            if (elapsed > static_cast<int>(1000 / Laser::FireRate))
            {
                m_lasers[m_currentLaser].shoot(m_player.getGunPosition());
                m_currentLaser++;
                if (m_currentLaser > m_lasers.size())
                {
                    m_currentLaser = 0;
                }
                m_lastFired = m_totalGameTime;
            }
        }
    } // end input while playing
}

/**
 * Update all object positions and check for collisions
 * Check for GameOver event (player dead)
 * @param dtSeconds time since last frame
 */
void Engine::update(const float dtSeconds)
{
    // only update during the actual game
    if (state != State::Playing)
    {
        return;
    }

    m_shroomMan.checkSpiderCollision(m_spider.getCollider());

    m_player.checkSpiderCollision(m_spider.getCollider());

    for (auto& laser : m_lasers)
    {
        // skip updating or colliding with inactive lasers
        if (!laser.isActive())
        {
            continue;
        }

        if (m_spider.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            continue;
        }

        if (m_shroomMan.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            continue;
        }

        if (m_centipede.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            continue;
        }

        laser.update(dtSeconds); // move the laser upward
    }

    m_centipede.update(dtSeconds);
    m_spider.update(dtSeconds);
    m_player.update(dtSeconds);

    // when the player dies, restart the game
    if (m_player.isDead())
    {
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

    if (state == State::Start)
    {
        // draw the start screen at beginning
        m_window.draw(m_startSprite);
    }
    else if (state == State::Playing)
    {
        // draw all the objects during game-play

        m_window.draw(m_spider);

        m_window.draw(m_shroomMan);

        // draw centipede(s)
        m_window.draw(m_centipede);

        // draw lasers (automatically doesn't draw inactive ones)
        for (const auto& laser : m_lasers)
        {
            m_window.draw(laser);
        }

        m_window.draw(m_player);
    }

    m_window.display();
}

/**
 * Handles resizing the SFML viewport to preserve the correct game aspect ratio
 * when the main window is resized. This prevents any distortion of the game characters,
 * while enlarging (or shrinking) uniformly.
 *
 * @param width, height new size of the main window
 */
void Engine::setViewport(unsigned int width, unsigned int height)
{
    float windowRatio = static_cast<float>(width) / static_cast<float>(height);
    float viewRatio   = m_view.getSize().x / m_view.getSize().y;
    float sizeX       = 1;
    float sizeY       = 1;
    float posX        = 0;
    float posY        = 0;

    if (windowRatio > viewRatio)
    {
        sizeX = viewRatio / windowRatio;
        posX  = (1 - sizeX) / 2;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY  = (1 - sizeY) / 2;
    }

    m_view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    m_window.setView(m_view);
}