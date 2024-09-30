#pragma once
#include <SFML/Graphics.hpp>

/** Settings and Constants for the Centipede game */
namespace Game
{

/** Name of the game for window title, debug, etc. */
inline constexpr auto& Name = "Centipede";

/** Play area is made up of 8x8 px grid */
inline constexpr int GridSize = 8.0;

/** Main play area is 30x30 grid, with an extra row on top and bottom. */
inline const sf::Vector2f GameSize{240, 256};

/** The view is centered so origin is top left (0,0). */
inline const sf::Vector2f GameCenter{GameSize / 2.0f};

}; // end namespace Game
