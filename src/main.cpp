/*
Author: Jackson Miller
Class: ECE6122 A
Last Date Modifed: 2024-09-30

Description:
Centipede Game using C++ and SFML.
*/
#include "Engine.hpp"

int main()
{
    try
    {
        Engine engine;
        engine.run();
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
