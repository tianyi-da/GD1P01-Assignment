//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        HumanPlayer.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the HumanPlayer class.
//==============================================================================================

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer()
    : Player("Player")
{
}

bool HumanPlayer::IsHuman() const
{
    return true;
}
