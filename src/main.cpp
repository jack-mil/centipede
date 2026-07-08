/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

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
