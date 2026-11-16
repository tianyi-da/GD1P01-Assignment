//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        HumanPlayer.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the HumanPlayer class. The human's decisions are made through the
//               keyboard, so this class only needs to identify itself as human.
//==============================================================================================

#pragma once

#include "Player.h"

class HumanPlayer : public Player
{
public:
    HumanPlayer();

    bool IsHuman() const override;
};
