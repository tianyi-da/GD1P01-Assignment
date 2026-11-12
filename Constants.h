//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Constants.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Shared game constants. Keeping these values in one place means the board size
//               or fleet composition can be changed without editing every class.
//==============================================================================================

#pragma once

// TF: Constant - a const value cannot be modified after it has been initialised.
const int BOARD_SIZE = 10;

const int FLEET_SIZE = 5;
const int TOTAL_SHIP_CELLS = 17;

// Characters used to draw the two grids.
const char CELL_WATER = '~';
const char CELL_HIT = 'X';
const char CELL_MISS = 'O';

// Safety limit for random ship placement so a full board can never cause an endless loop.
const int MAX_PLACEMENT_ATTEMPTS = 1000;
