#include <iostream>
#include "Engine.hpp"

// Window size constants
// Strange size to match given splash image
#define WIDTH 1036
#define HEIGHT 569

#define BOTTOM_HEIGHT HEIGHT / 5.f

static const auto worldColor = sf::Color(12, 24, 30, 255);

Engine::Engine() {
    sf::VideoMode vm(WIDTH, HEIGHT);
    m_Window.create(vm, "Centipede", sf::Style::Close);
    m_Window.setSize(sf::Vector2u(WIDTH, HEIGHT));
    m_Window.setVerticalSyncEnabled(false);
    // place the window in the center of the screen
    auto desktop = sf::VideoMode::getDesktopMode();
    m_Window.setPosition(sf::Vector2i((desktop.width / 2) - (vm.width / 2),
                                      (desktop.height / 2) - (vm.height / 2)));

    m_startSprite.setTexture(TextureHolder::GetTexture("graphics/startup-screen-background.png"));
    m_startSprite.setPosition(0.0, 0.0);

    m_playerBounds = sf::FloatRect(0, HEIGHT - BOTTOM_HEIGHT, WIDTH, BOTTOM_HEIGHT);
}

void Engine::run() {
    sf::Clock clock;
    while (m_Window.isOpen()) {
        sf::Time dt = clock.restart();
        m_TotalGameTime += dt;
        float dtSeconds = dt.asSeconds();

        input();
        update(dtSeconds);
        draw();
    }
}

void Engine::input() {
    // handle event polling for some inputs (start/end, etc)
    sf::Event event;
    while (m_Window.pollEvent(event)) {

        // Close the window when "X" button clicked
        if (event.type == sf::Event::Closed) {
            m_Window.close();
        }

        if (event.type == sf::Event::KeyPressed) {

            // Start game from "menu" with "ENTER"
            if (event.key.code == sf::Keyboard::Return && state == State::START) {
                // Position the player in bounds
                m_player.spawn(m_playerBounds);
                state = State::PLAYING;
                std::cout << "Started" << std::endl;
            }

            // Quit game whenever "ESC" pressed
            if (event.key.code == sf::Keyboard::Escape) {
                std::cout << "Ended" << std::endl;
                m_Window.close();
            }
        }
    } // end event polling

    // Keyboard polling for smooth player movement
    if (state == State::PLAYING) {
        // Handle player movement with WASD keys
        m_player.handleInput();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

            if (m_TotalGameTime.asMilliseconds() - m_lastFired.asMilliseconds() > 1000 / m_fireRate) {
                m_lasers[m_currentLaser].shoot(m_player.m_pos);
                m_currentLaser++;
                if (m_currentLaser > m_lasers.size()) {
                    m_currentLaser = 0;
                }
                m_lastFired = m_TotalGameTime;
            }
        }
    } // end input while playing
}

void Engine::update(const float dtSeconds) {
    if (state == State::PLAYING) {

        for (auto& laser : m_lasers) {
            if (laser.m_active) {
                laser.update(dtSeconds);
            }
        }

        m_player.update(dtSeconds);
    }
}

void Engine::draw() {
    m_Window.clear(worldColor);
    if (state == State::START) {
        m_Window.draw(m_startSprite);
    }
    if (state == State::PLAYING) {

        // draw lasers
        for (const auto& laser : m_lasers) {
            if (laser.m_active) {
                m_Window.draw(laser.m_shape);
            }
        }

        // draw starship
        m_Window.draw(m_player.m_Sprite);
    }
    m_Window.display();
}