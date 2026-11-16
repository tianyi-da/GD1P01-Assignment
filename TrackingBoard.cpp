//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        TrackingBoard.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the TrackingBoard class.
//==============================================================================================

#include "TrackingBoard.h"

TrackingBoard::TrackingBoard()
{
    Reset();
}

void TrackingBoard::Reset()
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            m_cells[row][column] = CELL_WATER;
        }
    }
}

bool TrackingBoard::IsTargeted(int row, int column) const
{
    if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
    {
        return true;
    }

    return m_cells[row][column] == CELL_HIT || m_cells[row][column] == CELL_MISS;
}

void TrackingBoard::Record(int row, int column, char symbol)
{
    if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
    {
        return;
    }

    if (symbol != CELL_HIT && symbol != CELL_MISS)
    {
        return;
    }

    m_cells[row][column] = symbol;
}

char TrackingBoard::GetCell(int row, int column) const
{
    if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
    {
        return '?';
    }

    return m_cells[row][column];
}
