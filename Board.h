//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Board.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the Board class. A Board is one player's 10 x 10 patch of ocean: it
//               holds that player's fleet, stores where shots have landed and reports the
//               result of every attack.
//==============================================================================================

#pragma once

#include <string>
#include <vector>

#include "Constants.h"
#include "Ship.h"

enum class ShotResult
{
    Invalid,
    Miss,
    Hit,
    Sunk
};

class Board
{
public:
    Board();

    void Reset();
    void PlaceFleetRandomly();

    bool CanPlaceShip(const Ship& ship, int row, int column, Orientation orientation) const;
    bool PlaceShip(const Ship& ship, int row, int column, Orientation orientation);
    ShotResult ReceiveShot(int row, int column, std::string& shipName);

    bool IsFleetDestroyed() const;
    int GetShipsAfloat() const;
    int GetSunkShipCount() const;

    char GetCell(int row, int column) const;
    const std::vector<Ship>& GetShips() const;
    Ship* FindShipAt(int row, int column);

private:
    char m_cells[BOARD_SIZE][BOARD_SIZE];       // TF: Array
    std::vector<Ship> m_ships;
};
