//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Player.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the abstract Player base class. It owns the player's home board and
//               tracking board, and keeps the shot statistics used by the summary screen.
//==============================================================================================

#pragma once

#include <string>

#include "Board.h"
#include "TrackingBoard.h"

class Player
{
public:
    explicit Player(const std::string& name);
    virtual ~Player();

    const std::string& GetName() const;

    Board& GetHomeBoard();
    const Board& GetHomeBoard() const;

    TrackingBoard& GetTrackingBoard();
    const TrackingBoard& GetTrackingBoard() const;

    virtual bool IsHuman() const = 0;

    void RecordShot(bool hit);
    int GetShotsFired() const;
    int GetHits() const;
    float GetAccuracy() const;

protected:
    std::string m_name;
    Board m_homeBoard;
    TrackingBoard m_trackingBoard;
    int m_shotsFired;
    int m_hits;
};
