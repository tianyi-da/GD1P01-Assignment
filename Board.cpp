//
// Bachelor of Software Engineering
// Media Design School at Strayer
// Auckland
// New Zealand
//
// (c) 2026 Media Design School at Strayer
//
// File Name    : Board.cpp
// Description  : Board implementation: placement, firing and drawing
// Author       : Your Name
// Mail         : your.name@mds.ac.nz
//

#include "Board.h"
#include <cstdlib>

namespace
{
const char WATER = '~';
const char SHIP = '#';
const char HIT = 'X';
const char MISS = 'o';

const char* const SHIP_NAMES[FLEET_SIZE] = { "Aircraft Carrier", "Battleship", "Destroyer", "Submarine", "Patrol Boat" };
const int SHIP_LENGTHS[FLEET_SIZE] = { 5, 4, 3, 3, 2 };

const char* const ANSI_RESET = "\033[0m";
const char* const ANSI_BLUE = "\033[34m";
const char* const ANSI_GREEN = "\033[32m";
const char* const ANSI_RED = "\033[1;31m";
const char* const ANSI_YELLOW = "\033[1;33m";
}

Board::Board()
{
    // TF: Dynamic Memory
    m_ships = new Ship[FLEET_SIZE];
    Reset();
}

Board::Board(const Board& other)
{
    m_ships = new Ship[FLEET_SIZE];
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            m_grid[r][c] = other.m_grid[r][c];
            m_shipAt[r][c] = other.m_shipAt[r][c];
        }
    }
    for (int i = 0; i < FLEET_SIZE; ++i)
    {
        m_ships[i] = other.m_ships[i];
    }
}

Board::~Board()
{
    delete[] m_ships;
    m_ships = nullptr;
}

void Board::Reset()
{
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            m_grid[r][c] = WATER;
            m_shipAt[r][c] = -1;
        }
    }
    for (int i = 0; i < FLEET_SIZE; ++i)
    {
        m_ships[i].Init(SHIP_NAMES[i], SHIP_LENGTHS[i]);
    }
}

bool Board::CanPlace(int shipIndex, int row, int col, bool horizontal) const
{
    if (shipIndex < 0 || shipIndex >= FLEET_SIZE)
    {
        return false;
    }
    const int length = m_ships[shipIndex].GetLength();
    const int endRow = horizontal ? row : row + length - 1;
    const int endCol = horizontal ? col + length - 1 : col;

    if (row < 0 || col < 0 || endRow >= BOARD_SIZE || endCol >= BOARD_SIZE)
    {
        return false;
    }
    for (int i = 0; i < length; ++i)
    {
        const int r = horizontal ? row : row + i;
        const int c = horizontal ? col + i : col;
        if (m_shipAt[r][c] != -1)
        {
            return false;   // overlaps another ship
        }
    }
    return true;
}

bool Board::PlaceShip(int shipIndex, int row, int col, bool horizontal)
{
    if (!CanPlace(shipIndex, row, col, horizontal) || m_ships[shipIndex].IsPlaced())
    {
        return false;
    }
    const int length = m_ships[shipIndex].GetLength();
    for (int i = 0; i < length; ++i)
    {
        const int r = horizontal ? row : row + i;
        const int c = horizontal ? col + i : col;
        m_grid[r][c] = SHIP;
        m_shipAt[r][c] = shipIndex;
    }
    m_ships[shipIndex].Place(row, col, horizontal);
    return true;
}

void Board::PlaceRandomly()
{
    Reset();
    for (int i = 0; i < FLEET_SIZE; ++i)
    {
        bool placed = false;
        while (!placed)
        {
            // TF: Pseudo Random Number
            const int row = rand() % BOARD_SIZE;
            const int col = rand() % BOARD_SIZE;
            const bool horizontal = (rand() % 2) == 0;
            placed = PlaceShip(i, row, col, horizontal);
        }
    }
}

ShotResult Board::Fire(int row, int col, int& sunkShipIndex)
{
    sunkShipIndex = -1;

    // TF: Relational Operator
    // TF: Logical Operator
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        return ShotResult::Invalid;
    }

    const char cell = m_grid[row][col];
    if (cell == HIT || cell == MISS)
    {
        return ShotResult::AlreadyShot;
    }
    if (cell == WATER)
    {
        m_grid[row][col] = MISS;
        return ShotResult::Miss;
    }

    m_grid[row][col] = HIT;
    const int index = m_shipAt[row][col];
    // TF: Pointer Initialised
    Ship* shipPtr = &m_ships[index];
    // TF: Pointer Dereferenced
    // TF: Reference
    Ship& hitShip = *shipPtr;
    hitShip.RegisterHit();

    if (hitShip.IsSunk())
    {
        sunkShipIndex = index;
        return ShotResult::Sunk;
    }
    return ShotResult::Hit;
}

bool Board::AllShipsSunk() const
{
    return ShipsAfloat() == 0;
}

int Board::ShipsAfloat() const
{
    int count = 0;
    for (int i = 0; i < FLEET_SIZE; ++i)
    {
        if (!m_ships[i].IsSunk())
        {
            ++count;
        }
    }
    return count;
}

bool Board::HasBeenShot(int row, int col) const
{
    return m_grid[row][col] == HIT || m_grid[row][col] == MISS;
}

const Ship& Board::GetShip(int index) const
{
    return m_ships[index];
}

std::string Board::Header()
{
    std::string header = "   ";
    for (int c = 0; c < BOARD_SIZE; ++c)
    {
        header += static_cast<char>('A' + c);
        header += ' ';
    }
    return header;
}

std::string Board::RowToString(int row, bool showShips) const
{
    std::string line;
    if (row + 1 < 10)
    {
        line += ' ';
    }
    line += std::to_string(row + 1);
    line += ' ';

    // TF: Iteration Structure
    for (int col = 0; col < BOARD_SIZE; ++col)
    {
        char cell = m_grid[row][col];
        if (cell == SHIP && !showShips)
        {
            cell = WATER;
        }

        const char* colour = ANSI_BLUE;
        // TF: Conditional Statement
        if (cell == SHIP)
        {
            colour = ANSI_GREEN;
        }
        else if (cell == HIT)
        {
            colour = ANSI_RED;
        }
        else if (cell == MISS)
        {
            colour = ANSI_YELLOW;
        }

        line += colour;
        line += cell;
        line += ANSI_RESET;
        line += ' ';
    }
    return line;
}
