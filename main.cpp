//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        main.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Entry point. Seeds the random number generator, creates the Game object and
//               starts the main menu loop.
//==============================================================================================

#include <cstdlib>
#include <ctime>

#include "Game.h"

int main()
{
    // Seed the pseudo random number generator once so ship placement differs each run.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // TF: Class Instance - game is an object created from the Game class.
    Game game;
    game.Run();

    return 0;
}
