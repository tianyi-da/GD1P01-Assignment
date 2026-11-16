//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        UI.h
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Declares the UI helper class. Every screen, colour, board drawing and input
//               routine lives here so Game can concentrate on the rules of Battleships.
//==============================================================================================

#pragma once

#include <string>
#include <vector>

#include "Board.h"
#include "TrackingBoard.h"

// Everything the battle screen needs to draw itself.
struct BattleStatus
{
    const Board* homeBoard;
    const TrackingBoard* trackingBoard;
    int turnNumber;
    int shotsFired;
    int hits;
    int shipsRemaining;
    bool debugMode;
    std::string message;
    std::vector<std::string> playerLosses;
    std::vector<std::string> enemyLosses;
};

class UI
{
public:
    static void Initialise();
    static void ClearScreen();

    static void ShowTitle();
    static void ShowMainMenu(bool debugMode);
    static void ShowInstructions();
    static void ShowPlacementScreen(const Board& board,
                                    const std::string& heading,
                                    const std::string& message);
    static void ShowBattleScreen(const BattleStatus& status);
    static void ShowDebugScreen(const Board& computerHomeBoard,
                                const TrackingBoard& computerTrackingBoard);
    static void ShowSummary(bool victory,
                            int shots,
                            int hits,
                            float accuracy,
                            char rating,
                            const std::vector<std::string>& playerLosses,
                            const std::vector<std::string>& enemyLosses);

    static std::string ReadLine(const std::string& prompt, bool& endOfInput);
    static int ReadMenuChoice(const std::string& prompt, int minChoice, int maxChoice, bool& endOfInput);
    static bool ParseCoordinate(const std::string& text, int& row, int& column);
    static void Pause(const std::string& message = "Press Enter to continue...");  // TF: Default Parameter
    static std::string ToUpper(const std::string& text);

private:
    enum class TextColour
    {
        Default,
        Cyan,
        Blue,
        Green,
        Red,
        Yellow,
        Grey,
        White
    };

    static std::string ColourCode(TextColour colour);
    static std::string Coloured(char cell, bool ownBoard);
    static std::string Centre(const std::string& text, int width);
    static std::string BoardColumnHeader();
    static std::vector<std::string> BuildBoardLines(const Board& board,
                                                    const std::string& title,
                                                    bool revealShips);
    static std::vector<std::string> BuildTrackingLines(const TrackingBoard& board,
                                                       const std::string& title);
    static void PrintSideBySide(const std::vector<std::string>& left,
                                const std::vector<std::string>& right);
    static void PrintFleetStatus(const Board& board);
    static void PrintWrappedList(const std::string& prefix,
                                 const std::vector<std::string>& items,
                                 int continuationIndent);
    static void PrintRule();

    static bool s_useColour;
};
