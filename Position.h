//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Position.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: A tiny value type that stores a row/column pair on the board. Using a struct
//               stops row and column values from being accidentally swapped in function calls.
//==============================================================================================

#pragma once

#include "Constants.h"

struct Position
{
    int row;
    int column;

    Position()
        : row(0), column(0)
    {
    }

    Position(int rowIndex, int columnIndex)
        : row(rowIndex), column(columnIndex)
    {
    }
};

// Returns true when the position is somewhere on the 10 x 10 board.
inline bool IsInsideBoard(const Position& position)
{
    return position.row >= 0 && position.row < BOARD_SIZE &&
           position.column >= 0 && position.column < BOARD_SIZE;
}
