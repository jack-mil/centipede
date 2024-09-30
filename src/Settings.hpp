#pragma once
#include <SFML/Graphics.hpp>

/** Settings and Constants for the Centipede game */
namespace Game {

    /** Name of the game for window title, debug, etc. */
    inline constexpr auto& Name = "Centipede";

    /** Play area is made up of 8x8 px grid */
    inline constexpr int GridSize = 8;

    /** Main play area is 30x30 grid, with an extra row on top and bottom. */
    inline const sf::Vector2f GameSize{240, 256};

    /** The view is centered so origin is top left (0,0). */
    inline const sf::Vector2f GameCenter{GameSize / 2.0f};

    /** The actual OS window can be larger, sprites are scaled up. */
    inline const sf::Vector2u WindowSize{static_cast<sf::Vector2u>(3.5f * GameSize)};

    /** The OS window mode is just it's size and bit-depth (no constructor from vector?) */
    inline const sf::VideoMode WindowMode{WindowSize.x, WindowSize.y};

};  // end namespace Game
