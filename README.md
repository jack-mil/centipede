# Atari Centipede Clone

## Made with SFML and C++17

- configure: `cmake -B build/`
- compile:   `cmake --build build/`
- run: `./build/bin/centipede`

CMake will automatically clone and build the SFML dependency.


## Details
Uses the original sprite sheet from the 1981 arcade game. There is a lot I want to add, but was too large a scope for regular class assignment. May revisit this at some point.

Current features are only the minimal required by the grading rubric, there is still a bit to be done before I would consider it a 'full game'.

Future features I would like to have:
- Score system
- HUD overlay for lives / score counter
- Centipede collision with the player
- Animations for all characters
- Change the laser system to that of the original game (only one laser on screen at a time)
- Redesign of the game loop, cleanup of tick based simulation

Big thanks to these blog posts by the RetroGame Deconstruction Zone:
- [Character Breakdown](https://www.retrogamedeconstructionzone.com/2020/08/the-characters-of-centipede.html)
- [Descent Mechanics](https://www.retrogamedeconstructionzone.com/2020/11/the-descent-of-centipede-part-1.html)