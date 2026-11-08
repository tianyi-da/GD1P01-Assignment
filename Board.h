//
// Bachelor of Software Engineering
// Media Design School at Strayer
// Auckland
// New Zealand
//
// (c) 2026 Media Design School at Strayer
//
// File Name    : Board.h
// Description  : 10x10 board holding a fleet of ships, shots and rendering
// Author       : Your Name
// Mail         : your.name@mds.ac.nz
//

#pragma once

#include <string>
#include "Ship.h"

enum class ShotResult
{
    Invalid,
    AlreadyShot,
    Miss,
    Hit,
    Sunk
};

// TF: Class
class Board
{
// TF: Access Specifier
public:
    // TF: Constructor
    Board();
    // TF: Copy Constructor
    Board(const Board& other);
    // TF: Destructor
    ~Board();
    Board& operator=(const Board&) = delete;

    void Reset();
    bool CanPlace(int shipIndex, int row, int col, bool horizontal) const;
    bool PlaceShip(int shipIndex, int row, int col, bool horizontal);
    void PlaceRandomly();

    ShotResult Fire(int row, int col, int& sunkShipIndex);
    bool AllShipsSunk() const;
    int ShipsAfloat() const;
    bool HasBeenShot(int row, int col) const;
    const Ship& GetShip(int index) const;

    // Returns one coloured text row of the grid. Ships are hidden unless showShips is true.
    // TF: Default Parameter
    std::string RowToString(int row, bool showShips = true) const;
    static std::string Header();

private:
    // TF: Array
    char m_grid[BOARD_SIZE][BOARD_SIZE];
    int m_shipAt[BOARD_SIZE][BOARD_SIZE];   // ship index occupying a cell, or -1
    Ship* m_ships;                          // dynamically allocated fleet
};
