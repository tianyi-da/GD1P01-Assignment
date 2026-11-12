//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Ship.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the Ship class, including the rule of three so the dynamically
//               allocated hit map is copied and released correctly.
//==============================================================================================

#include "Ship.h"

#include <utility>

Ship::Ship()
    : m_name("None"),
      m_symbol('?'),
      m_length(0),
      m_row(-1),
      m_column(-1),
      m_orientation(Orientation::Horizontal),
      m_placed(false),
      m_hits(0)
{
}

// TF: Constructor - builds a fully initialised ship and allocates its hit map.
Ship::Ship(const std::string& name, char symbol, int length)
    : m_name(name),
      m_symbol(symbol),
      m_length(length > 0 ? length : 0),
      m_row(-1),
      m_column(-1),
      m_orientation(Orientation::Horizontal),
      m_placed(false),
      m_hits(0)
{
    // TF: Dynamic Memory - the hit map is created at run time and lives until the ship dies.
    m_hitMap = new bool[m_length];

    for (int index = 0; index < m_length; ++index)
    {
        m_hitMap[index] = false;
    }
}

// TF: Copy Constructor - makes a deep copy so two ships never share one hit map.
Ship::Ship(const Ship& other)
    : m_name(other.m_name),
      m_symbol(other.m_symbol),
      m_length(other.m_length),
      m_row(other.m_row),
      m_column(other.m_column),
      m_orientation(other.m_orientation),
      m_placed(other.m_placed),
      m_hits(other.m_hits),
      m_hitMap(nullptr)
{
    if (m_length > 0)
    {
        m_hitMap = new bool[m_length];

        for (int index = 0; index < m_length; ++index)
        {
            m_hitMap[index] = other.m_hitMap[index];
        }
    }
}

Ship& Ship::operator=(const Ship& other)
{
    if (this == &other)
    {
        return *this;
    }

    bool* newHitMap = nullptr;

    if (other.m_length > 0)
    {
        newHitMap = new bool[other.m_length];

        for (int index = 0; index < other.m_length; ++index)
        {
            newHitMap[index] = other.m_hitMap[index];
        }
    }

    delete[] m_hitMap;

    m_hitMap = newHitMap;
    m_name = other.m_name;
    m_symbol = other.m_symbol;
    m_length = other.m_length;
    m_row = other.m_row;
    m_column = other.m_column;
    m_orientation = other.m_orientation;
    m_placed = other.m_placed;
    m_hits = other.m_hits;

    return *this;
}

// TF: Destructor - releases the memory owned by this ship.
Ship::~Ship()
{
    delete[] m_hitMap;
    m_hitMap = nullptr;
}

void Ship::SetPosition(int row, int column, Orientation orientation)
{
    m_row = row;
    m_column = column;
    m_orientation = orientation;
    m_placed = true;
    m_hits = 0;

    for (int index = 0; index < m_length; ++index)
    {
        m_hitMap[index] = false;
    }
}

void Ship::Reset()
{
    m_row = -1;
    m_column = -1;
    m_placed = false;
    m_hits = 0;

    for (int index = 0; index < m_length; ++index)
    {
        m_hitMap[index] = false;
    }
}

bool Ship::Occupies(int row, int column) const
{
    if (!m_placed)
    {
        return false;
    }

    if (m_orientation == Orientation::Horizontal)
    {
        return row == m_row &&
               column >= m_column &&
               column < m_column + m_length;
    }

    return column == m_column &&
           row >= m_row &&
           row < m_row + m_length;
}

bool Ship::RegisterHit(int row, int column)
{
    if (!m_placed || !Occupies(row, column) || m_hitMap == nullptr)
    {
        return false;
    }

    // TF: Arithmetic Operator - subtracting the ship's start square gives the hit index.
    const int index = (m_orientation == Orientation::Horizontal)
                      ? (column - m_column)
                      : (row - m_row);

    if (index < 0 || index >= m_length)
    {
        return false;
    }

    if (*(m_hitMap + index))                // TF: Pointer Dereferenced
    {
        return false;
    }

    *(m_hitMap + index) = true;
    ++m_hits;

    return true;
}

bool Ship::IsSunk() const
{
    // TF: Relational Operator - the ship is sunk once its hits reach its length.
    return m_placed && m_length > 0 && m_hits >= m_length;
}

bool Ship::IsPlaced() const
{
    return m_placed;
}

const std::string& Ship::GetName() const
{
    return m_name;
}

char Ship::GetSymbol() const
{
    return m_symbol;
}

int Ship::GetLength() const
{
    return m_length;
}

int Ship::GetRow() const
{
    return m_row;
}

int Ship::GetColumn() const
{
    return m_column;
}

int Ship::GetHits() const
{
    return m_hits;
}

Orientation Ship::GetOrientation() const
{
    return m_orientation;
}
