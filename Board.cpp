//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Board.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the Board class: fleet placement, shot resolution and fleet status.
//==============================================================================================

#include "Board.h"

#include <cstdlib>

namespace
{
    const char* const SHIP_NAMES[FLEET_SIZE] =
    {
        "Aircraft Carrier",
        "Battleship",
        "Destroyer",
        "Submarine",
        "Patrol Boat"
    };

    const char SHIP_SYMBOLS[FLEET_SIZE] = { 'A', 'B', 'D', 'S', 'P' };
    const int SHIP_LENGTHS[FLEET_SIZE] = { 5, 4, 3, 3, 2 };
}

Board::Board()
{
    Reset();
}

void Board::Reset()
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            m_cells[row][column] = CELL_WATER;
        }
    }

    // Clearing the vector destroys every Ship, which frees each ship's hit map.
    m_ships.clear();
}

bool Board::CanPlaceShip(const Ship& ship, int row, int column, Orientation orientation) const
{
    if (ship.GetLength() <= 0)
    {
        return false;
    }

    for (int offset = 0; offset < ship.GetLength(); ++offset)
    {
        const int currentRow = row + (orientation == Orientation::Vertical ? offset : 0);
        const int currentColumn = column + (orientation == Orientation::Horizontal ? offset : 0);

        // TF: Logical Operator - || rejects the position if any single part fails.
        if (currentRow < 0 || currentRow >= BOARD_SIZE ||
            currentColumn < 0 || currentColumn >= BOARD_SIZE)
        {
            return false;
        }

        if (m_cells[currentRow][currentColumn] != CELL_WATER)
        {
            return false;
        }
    }

    return true;
}

bool Board::PlaceShip(const Ship& ship, int row, int column, Orientation orientation)
{
    if (!CanPlaceShip(ship, row, column, orientation))
    {
        return false;
    }

    // The ship is copied into the fleet; Ship's copy constructor handles the deep copy.
    m_ships.push_back(ship);

    // TF: Reference - placed is another name for the ship that now lives in the vector.
    Ship& placed = m_ships.back();
    placed.SetPosition(row, column, orientation);

    for (int offset = 0; offset < placed.GetLength(); ++offset)
    {
        const int currentRow = placed.GetRow() +
                               (orientation == Orientation::Vertical ? offset : 0);
        const int currentColumn = placed.GetColumn() +
                                  (orientation == Orientation::Horizontal ? offset : 0);

        m_cells[currentRow][currentColumn] = placed.GetSymbol();
    }

    return true;
}

void Board::PlaceFleetRandomly()
{
    for (int attempt = 0; attempt < 10; ++attempt)
    {
        Reset();

        bool fleetComplete = true;

        // TF: Iteration Structure - a for loop visits every ship in the fleet.
        for (int index = 0; index < FLEET_SIZE; ++index)
        {
            Ship ship(SHIP_NAMES[index], SHIP_SYMBOLS[index], SHIP_LENGTHS[index]);
            bool placed = false;
            int tries = 0;

            while (!placed && tries < MAX_PLACEMENT_ATTEMPTS)
            {
                // TF: Pseudo Random Number - rand() picks a start square and an orientation.
                const int row = std::rand() % BOARD_SIZE;
                const int column = std::rand() % BOARD_SIZE;
                const Orientation orientation = (std::rand() % 2 == 0)
                                                ? Orientation::Horizontal
                                                : Orientation::Vertical;

                placed = PlaceShip(ship, row, column, orientation);
                ++tries;
            }

            if (!placed)
            {
                // Deterministic fallback: scan the board until a legal position is found.
                for (int row = 0; row < BOARD_SIZE && !placed; ++row)
                {
                    for (int column = 0; column < BOARD_SIZE && !placed; ++column)
                    {
                        for (int choice = 0; choice < 2 && !placed; ++choice)
                        {
                            const Orientation orientation = (choice == 0)
                                                            ? Orientation::Horizontal
                                                            : Orientation::Vertical;

                            placed = PlaceShip(ship, row, column, orientation);
                        }
                    }
                }
            }

            if (!placed)
            {
                fleetComplete = false;
                break;
            }
        }

        if (fleetComplete && GetShipsAfloat() == FLEET_SIZE)
        {
            return;
        }
    }

    // Last resort that always fits: five rows, all horizontal, starting in column A.
    Reset();

    for (int index = 0; index < FLEET_SIZE; ++index)
    {
        Ship ship(SHIP_NAMES[index], SHIP_SYMBOLS[index], SHIP_LENGTHS[index]);
        PlaceShip(ship, index * 2, 0, Orientation::Horizontal);
    }
}

ShotResult Board::ReceiveShot(int row, int column, std::string& shipName)
{
    shipName.clear();

    if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
    {
        return ShotResult::Invalid;
    }

    char& cell = m_cells[row][column];

    if (cell == CELL_HIT || cell == CELL_MISS)
    {
        return ShotResult::Invalid;
    }

    Ship* ship = FindShipAt(row, column);

    if (ship == nullptr)
    {
        cell = CELL_MISS;
        return ShotResult::Miss;
    }

    // TF: Conditional Statement - the result depends on whether the ship is now sunk.
    if (ship->RegisterHit(row, column) && ship->IsSunk())
    {
        cell = CELL_HIT;
        shipName = ship->GetName();
        return ShotResult::Sunk;
    }

    cell = CELL_HIT;
    shipName = ship->GetName();
    return ShotResult::Hit;
}

bool Board::IsFleetDestroyed() const
{
    return !m_ships.empty() && GetShipsAfloat() == 0;
}

int Board::GetShipsAfloat() const
{
    int afloat = 0;

    for (std::size_t index = 0; index < m_ships.size(); ++index)
    {
        if (!m_ships[index].IsSunk())
        {
            ++afloat;
        }
    }

    return afloat;
}

int Board::GetSunkShipCount() const
{
    return static_cast<int>(m_ships.size()) - GetShipsAfloat();
}

char Board::GetCell(int row, int column) const
{
    if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
    {
        return '?';
    }

    return m_cells[row][column];
}

const std::vector<Ship>& Board::GetShips() const
{
    return m_ships;
}

Ship* Board::FindShipAt(int row, int column)
{
    for (std::size_t index = 0; index < m_ships.size(); ++index)
    {
        if (m_ships[index].Occupies(row, column))
        {
            return &m_ships[index];
        }
    }

    return nullptr;
}
