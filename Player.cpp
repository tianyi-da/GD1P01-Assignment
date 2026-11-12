//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Player.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the shared behaviour of the human player and the computer player.
//==============================================================================================

#include "Player.h"

Player::Player(const std::string& name)
    : m_name(name),
      m_homeBoard(),
      m_trackingBoard(),
      m_shotsFired(0),
      m_hits(0)
{
}

Player::~Player()
{
}

const std::string& Player::GetName() const
{
    return m_name;
}

Board& Player::GetHomeBoard()
{
    return m_homeBoard;
}

const Board& Player::GetHomeBoard() const
{
    return m_homeBoard;
}

TrackingBoard& Player::GetTrackingBoard()
{
    return m_trackingBoard;
}

const TrackingBoard& Player::GetTrackingBoard() const
{
    return m_trackingBoard;
}

void Player::RecordShot(bool hit)
{
    ++m_shotsFired;

    if (hit)
    {
        ++m_hits;
    }
}

int Player::GetShotsFired() const
{
    return m_shotsFired;
}

int Player::GetHits() const
{
    return m_hits;
}

float Player::GetAccuracy() const
{
    if (m_shotsFired == 0)
    {
        return 0.0f;
    }

    return (static_cast<float>(m_hits) * 100.0f) / static_cast<float>(m_shotsFired);
}
