//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Ship.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the Ship class. A ship knows its name, its length, where it sits on
//               the board and which of its squares have been hit. The hit map is allocated
//               dynamically so ships of any length can be supported.
//==============================================================================================

#pragma once

#include <string>

enum class Orientation
{
    Horizontal,
    Vertical
};

// TF: Class - a class bundles data and the operations that work on that data.
class Ship
{
public:
    Ship();
    Ship(const std::string& name, char symbol, int length);
    Ship(const Ship& other);
    Ship& operator=(const Ship& other);
    ~Ship();

    void SetPosition(int row, int column, Orientation orientation);
    void Reset();

    bool Occupies(int row, int column) const;
    bool RegisterHit(int row, int column);
    bool IsSunk() const;
    bool IsPlaced() const;

    const std::string& GetName() const;
    char GetSymbol() const;
    int GetLength() const;
    int GetRow() const;
    int GetColumn() const;
    int GetHits() const;
    Orientation GetOrientation() const;

private:
    // TF: Access Specifier - private keeps the ship's data hidden behind its public functions.
    std::string m_name;
    char m_symbol;
    int m_length;
    int m_row;
    int m_column;
    Orientation m_orientation;
    bool m_placed;
    int m_hits;

    bool* m_hitMap = nullptr;                   // TF: Pointer Initialised
};
