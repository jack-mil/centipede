/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modified: 2024-09-11

Description:
Centipede Game using C++ and SFML.
*/
#include "Engine.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
    try {
        Engine engine;
        engine.run();
    } catch (const std::exception& e) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
