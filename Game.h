//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Game.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the Game class that controls the main menu, fleet deployment, the
//               turn based battle and the end of game summary.
//==============================================================================================

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AIPlayer.h"
#include "HumanPlayer.h"

class Game
{
public:
    Game();

    void Run();

private:
    enum class BattleOutcome
    {
        PlayerVictory,
        ComputerVictory,
        QuitToMenu
    };

    void StartNewGame();
    bool SetupFleets();
    bool ManualPlacement();
    BattleOutcome Battle();
    bool HumanTurn();
    void ComputerTurn();
    void ShowDebugView();
    bool FinishGame(bool victory);
    bool ConfirmQuit();
    void RedrawBattle();
    std::string FormatSquare(int row, int column) const;

    bool m_running;
    bool m_debugMode;
    bool m_endOfInput;
    int m_turnNumber;

    std::string m_message;
    std::vector<std::string> m_playerLosses;
    std::vector<std::string> m_enemyLosses;

    std::unique_ptr<HumanPlayer> m_player;
    std::unique_ptr<AIPlayer> m_computer;
};
