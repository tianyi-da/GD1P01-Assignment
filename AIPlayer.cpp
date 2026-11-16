//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        AIPlayer.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the computer opponent's targeting logic.
//==============================================================================================

#include "AIPlayer.h"

#include <cstdlib>

AIPlayer::AIPlayer()
    : Player("Computer"),
      m_targets()
{
}

bool AIPlayer::IsHuman() const
{
    return false;
}

Position AIPlayer::ChooseTarget()
{
    // First use any squares queued around a previous hit.
    while (!m_targets.empty())
    {
        const Position candidate = m_targets.back();
        m_targets.pop_back();

        if (IsInsideBoard(candidate) &&
            !GetTrackingBoard().IsTargeted(candidate.row, candidate.column))
        {
            return candidate;
        }
    }

    // Hunt on a checkerboard pattern: every ship of length two or more touches that pattern.
    std::vector<Position> candidates;

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            const bool parityMatch = ((row + column) % 2 == 0);

            if (parityMatch && !GetTrackingBoard().IsTargeted(row, column))
            {
                candidates.push_back(Position(row, column));
            }
        }
    }

    // If only odd-parity squares remain, widen the search.
    if (candidates.empty())
    {
        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            for (int column = 0; column < BOARD_SIZE; ++column)
            {
                if (!GetTrackingBoard().IsTargeted(row, column))
                {
                    candidates.push_back(Position(row, column));
                }
            }
        }
    }

    if (candidates.empty())
    {
        return Position(-1, -1);
    }

    const int index = std::rand() % static_cast<int>(candidates.size());
    return candidates[index];
}

void AIPlayer::ReportResult(const Position& position, ShotResult result)
{
    if (result == ShotResult::Hit || result == ShotResult::Sunk)
    {
        QueueNeighbours(position);
    }
}

void AIPlayer::QueueNeighbours(const Position& position)
{
    const int rowOffsets[4] = { -1, 1, 0, 0 };
    const int columnOffsets[4] = { 0, 0, -1, 1 };

    for (int index = 0; index < 4; ++index)
    {
        const Position neighbour(position.row + rowOffsets[index],
                                 position.column + columnOffsets[index]);

        if (IsInsideBoard(neighbour) &&
            !GetTrackingBoard().IsTargeted(neighbour.row, neighbour.column))
        {
            m_targets.push_back(neighbour);
        }
    }
}
