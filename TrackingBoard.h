//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        TrackingBoard.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the TrackingBoard class. This is the second grid each player keeps:
//               a record of every shot fired at the opponent, showing hits and misses only.
//==============================================================================================

#pragma once

#include "Constants.h"

class TrackingBoard
{
public:
    TrackingBoard();

    void Reset();
    bool IsTargeted(int row, int column) const;
    void Record(int row, int column, char symbol);
    char GetCell(int row, int column) const;

private:
    char m_cells[BOARD_SIZE][BOARD_SIZE];
};
