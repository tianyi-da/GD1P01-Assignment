//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        AIPlayer.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the AIPlayer class. The computer hunts on a checkerboard pattern and,
//               whenever it hits a ship, queues the surrounding squares so it can finish the
//               job. It never reads the human player's board, so it plays fairly.
//==============================================================================================

#pragma once

#include <vector>

#include "Player.h"
#include "Position.h"

class AIPlayer : public Player
{
public:
    AIPlayer();

    bool IsHuman() const override;

    Position ChooseTarget();
    void ReportResult(const Position& position, ShotResult result);

private:
    void QueueNeighbours(const Position& position);

    std::vector<Position> m_targets;
};
