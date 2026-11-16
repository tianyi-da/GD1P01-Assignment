//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        UI.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements every console screen and all keyboard input for the game.
//==============================================================================================

#include "UI.h"

#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace
{
    const int SCREEN_WIDTH = 74;
    const int BOARD_DISPLAY_WIDTH = BOARD_SIZE * 3 + 5;

    std::string Trim(const std::string& text)
    {
        const std::string whitespace = " \t\r\n";
        const std::size_t first = text.find_first_not_of(whitespace);

        if (first == std::string::npos)
        {
            return std::string();
        }

        const std::size_t last = text.find_last_not_of(whitespace);
        return text.substr(first, last - first + 1);
    }
}

bool UI::s_useColour = false;

void UI::Initialise()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitleA("Battleships - GD1P01 Final Project");

    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    if (output != INVALID_HANDLE_VALUE && GetConsoleMode(output, &mode) != 0)
    {
        if (SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0)
        {
            s_useColour = true;
        }
    }
#else
    s_useColour = true;
#endif
}

void UI::ClearScreen()
{
    if (s_useColour)
    {
        std::cout << "\x1b[2J\x1b[H";
    }
    else
    {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    std::cout.flush();
}

std::string UI::ColourCode(TextColour colour)
{
    if (!s_useColour)
    {
        return std::string();
    }

    switch (colour)
    {
        case TextColour::Cyan:  return "\x1b[96m";
        case TextColour::Blue:  return "\x1b[94m";
        case TextColour::Green: return "\x1b[92m";
        case TextColour::Red:   return "\x1b[91m";
        case TextColour::Yellow:return "\x1b[93m";
        case TextColour::Grey:  return "\x1b[90m";
        case TextColour::White: return "\x1b[97m";
        default:                return "\x1b[0m";
    }
}

std::string UI::Coloured(char cell, bool ownBoard)
{
    if (!s_useColour)
    {
        return std::string(1, cell);
    }

    switch (cell)
    {
        case CELL_WATER:
            return ColourCode(TextColour::Blue) + cell + ColourCode(TextColour::Default);

        case CELL_MISS:
            return ColourCode(TextColour::Yellow) + cell + ColourCode(TextColour::Default);

        case CELL_HIT:
            return ColourCode(TextColour::Red) + cell + ColourCode(TextColour::Default);

        default:
            if (ownBoard)
            {
                return ColourCode(TextColour::Green) + cell + ColourCode(TextColour::Default);
            }

            return std::string(1, cell);
    }
}

std::string UI::Centre(const std::string& text, int width)
{
    int padding = width - static_cast<int>(text.size());

    if (padding < 0)
    {
        padding = 0;
    }

    return std::string(padding / 2, ' ') + text;
}

std::string UI::BoardColumnHeader()
{
    std::string header = "    ";

    for (int column = 0; column < BOARD_SIZE; ++column)
    {
        if (column < 9)
        {
            header += " " + std::to_string(column + 1) + " ";
        }
        else
        {
            header += "10 ";
        }
    }

    header += " ";
    return header;
}

std::vector<std::string> UI::BuildBoardLines(const Board& board,
                                             const std::string& title,
                                             bool revealShips)
{
    std::vector<std::string> lines;
    lines.push_back(Centre(title, BOARD_DISPLAY_WIDTH));
    lines.push_back(BoardColumnHeader());
    lines.push_back("   +" + std::string(BOARD_SIZE * 3, '-') + "+");

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        std::string line = " ";
        line += static_cast<char>('A' + row);
        line += " |";

        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            char cell = board.GetCell(row, column);

            if (!revealShips &&
                cell != CELL_WATER &&
                cell != CELL_HIT &&
                cell != CELL_MISS)
            {
                cell = CELL_WATER;
            }

            line += " ";
            line += Coloured(cell, true);
            line += " ";
        }

        line += "|";
        lines.push_back(line);
    }

    lines.push_back("   +" + std::string(BOARD_SIZE * 3, '-') + "+");
    return lines;
}

std::vector<std::string> UI::BuildTrackingLines(const TrackingBoard& board,
                                                const std::string& title)
{
    std::vector<std::string> lines;
    lines.push_back(Centre(title, BOARD_DISPLAY_WIDTH));
    lines.push_back(BoardColumnHeader());
    lines.push_back("   +" + std::string(BOARD_SIZE * 3, '-') + "+");

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        std::string line = " ";
        line += static_cast<char>('A' + row);
        line += " |";

        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            line += " ";
            line += Coloured(board.GetCell(row, column), false);
            line += " ";
        }

        line += "|";
        lines.push_back(line);
    }

    lines.push_back("   +" + std::string(BOARD_SIZE * 3, '-') + "+");
    return lines;
}

void UI::PrintSideBySide(const std::vector<std::string>& left,
                         const std::vector<std::string>& right)
{
    const std::size_t count = left.size() > right.size() ? left.size() : right.size();

    for (std::size_t index = 0; index < count; ++index)
    {
        std::cout << "  ";

        if (index < left.size())
        {
            std::cout << left[index];
        }

        if (index < right.size())
        {
            std::cout << "  " << right[index];
        }

        std::cout << "\n";
    }
}

void UI::PrintRule()
{
    std::cout << "  " << ColourCode(TextColour::Cyan)
              << std::string(SCREEN_WIDTH, '=')
              << ColourCode(TextColour::Default) << "\n";
}

void UI::PrintFleetStatus(const Board& board)
{
    const std::vector<Ship>& ships = board.GetShips();

    if (ships.empty())
    {
        std::cout << "not deployed";
        return;
    }

    for (std::size_t index = 0; index < ships.size(); ++index)
    {
        const Ship& ship = ships[index];
        std::string label = std::string(1, ship.GetSymbol()) + ":" +
                            std::to_string(ship.GetHits()) + "/" +
                            std::to_string(ship.GetLength());

        if (ship.IsSunk())
        {
            std::cout << ColourCode(TextColour::Red) << label << " SUNK"
                      << ColourCode(TextColour::Default) << "   ";
        }
        else
        {
            std::cout << label << "   ";
        }
    }
}

void UI::PrintWrappedList(const std::string& prefix,
                          const std::vector<std::string>& items,
                          int continuationIndent)
{
    if (items.empty())
    {
        std::cout << prefix << "None\n";
        return;
    }

    std::string line = prefix;

    for (std::size_t index = 0; index < items.size(); ++index)
    {
        std::string piece = items[index];

        if (index + 1 < items.size())
        {
            piece += ",";
        }

        const std::string separator = (index == 0) ? std::string() : std::string(" ");

        if (index > 0 &&
            static_cast<int>(line.size() + separator.size() + piece.size()) > SCREEN_WIDTH)
        {
            std::cout << line << "\n";
            line.assign(static_cast<std::size_t>(continuationIndent), ' ');
        }
        else
        {
            line += separator;
        }

        line += piece;
    }

    std::cout << line << "\n";
}

void UI::ShowTitle()
{
    std::cout << "\n";
    std::cout << "  " << ColourCode(TextColour::Cyan);
    std::cout << R"( ____        _   _   _           _     _     )" << "\n";
    std::cout << R"(| __ )  __ _| |_| |_| | ___  ___| |__ (_)___ )" << "\n";
    std::cout << R"(|  _ \ / _` | __| __| |/ _ \/ __| '_ \| / __|)" << "\n";
    std::cout << R"(| |_) | (_| | |_| |_| |  __/\__ \ | | | \__ \)" << "\n";
    std::cout << R"(|____/ \__,_|\__|\__|_|\___||___/_| |_|_|___/)" << "\n";
    std::cout << ColourCode(TextColour::Default);
    std::cout << "\n";
    std::cout << "   GD1P01 - Introduction to Software Engineering   |   Final Project\n";
    std::cout << "   Single player Battleship against a computer opponent\n\n";
}

void UI::ShowMainMenu(bool debugMode)
{
    ShowTitle();
    PrintRule();
    std::cout << "\n";
    std::cout << "     1.   New Game\n";
    std::cout << "     2.   Instructions\n";
    std::cout << "     3.   Debug Mode .................... ";
    std::cout << (debugMode ? ColourCode(TextColour::Green) + std::string("[ ON ]")
                            : ColourCode(TextColour::Grey) + std::string("[ OFF ]"));
    std::cout << ColourCode(TextColour::Default) << "\n";
    std::cout << "     4.   Exit\n";
    std::cout << "\n";
    PrintRule();
    std::cout << "\n";
    std::cout << ColourCode(TextColour::Grey)
              << "   Debug Mode reveals the computer's two grids during a battle."
              << ColourCode(TextColour::Default) << "\n\n";
}

void UI::ShowInstructions()
{
    ClearScreen();
    std::cout << "\n";
    std::cout << "  " << ColourCode(TextColour::Cyan)
              << Centre("INSTRUCTIONS", SCREEN_WIDTH)
              << ColourCode(TextColour::Default) << "\n";
    PrintRule();
    std::cout << "\n";
    std::cout << "   OBJECTIVE\n";
    std::cout << "   Sink all five ships of the computer's fleet before it sinks yours.\n\n";
    std::cout << "   YOUR TWO GRIDS\n";
    std::cout << "   YOUR WATERS  - your ships and the shots the computer fired at you.\n";
    std::cout << "   ENEMY WATERS - your record of the shots you fired at the computer.\n\n";
    std::cout << "   SYMBOLS\n";
    std::cout << "   ~ water / not targeted     O miss     X hit\n";
    std::cout << "   A Aircraft Carrier (5)   B Battleship (4)   D Destroyer (3)\n";
    std::cout << "   S Submarine (3)          P Patrol Boat (2)\n\n";
    std::cout << "   DEPLOYING YOUR FLEET\n";
    std::cout << "   Random placement fills the board for you. Manual placement asks for\n";
    std::cout << "   each ship's orientation (H = across, V = down) and start square.\n";
    std::cout << "   Ships may not overlap or leave the board.\n\n";
    std::cout << "   TAKING A SHOT\n";
    std::cout << "   Type a letter and a number, for example B7 or J10, then press Enter.\n";
    std::cout << "   A ship sinks when every one of its squares has been hit.\n\n";
    std::cout << "   CONTROLS DURING BATTLE\n";
    std::cout << "   A1 - J10  fire at that square      H  show this help\n";
    std::cout << "   D         toggle Debug Mode        Q  quit to the main menu\n\n";
    PrintRule();
    std::cout << "\n";
}

void UI::ShowPlacementScreen(const Board& board,
                             const std::string& heading,
                             const std::string& message)
{
    ClearScreen();
    std::cout << "\n";
    std::cout << "  " << ColourCode(TextColour::Cyan)
              << Centre(heading, SCREEN_WIDTH)
              << ColourCode(TextColour::Default) << "\n";
    PrintRule();

    if (!message.empty())
    {
        std::cout << "\n   " << message << "\n";
    }

    std::cout << "\n";
    const std::vector<std::string> lines = BuildBoardLines(board, "YOUR WATERS", true);

    for (std::size_t index = 0; index < lines.size(); ++index)
    {
        std::cout << "  " << lines[index] << "\n";
    }

    std::cout << "\n   " << ColourCode(TextColour::Grey) << "Your fleet: "
              << ColourCode(TextColour::Default);
    PrintFleetStatus(board);
    std::cout << "\n\n";
}

void UI::ShowBattleScreen(const BattleStatus& status)
{
    if (status.homeBoard == nullptr || status.trackingBoard == nullptr)
    {
        return;
    }

    ClearScreen();
    std::cout << "\n";
    PrintRule();
    std::cout << "   " << ColourCode(TextColour::Cyan) << "BATTLE"
              << ColourCode(TextColour::Default)
              << "   |   Round " << status.turnNumber
              << "   |   Shots: " << status.shotsFired
              << "   |   Hits: " << status.hits;

    if (status.shotsFired > 0)
    {
        std::ostringstream accuracy;
        accuracy << std::fixed << std::setprecision(1)
                 << (static_cast<float>(status.hits) * 100.0f /
                     static_cast<float>(status.shotsFired));
        std::cout << "   |   Accuracy: " << accuracy.str() << "%";
    }

    std::cout << "\n";
    std::cout << "   Enemy ships afloat: " << status.shipsRemaining << "/" << FLEET_SIZE;

    if (status.debugMode)
    {
        std::cout << "     " << ColourCode(TextColour::Red)
                  << "[DEBUG MODE ON - press D]"
                  << ColourCode(TextColour::Default);
    }

    std::cout << "\n";
    PrintRule();
    std::cout << "\n";

    const std::vector<std::string> homeLines =
        BuildBoardLines(*status.homeBoard, "YOUR WATERS", true);
    const std::vector<std::string> trackingLines =
        BuildTrackingLines(*status.trackingBoard, "ENEMY WATERS");

    PrintSideBySide(homeLines, trackingLines);

    std::cout << "\n   " << ColourCode(TextColour::Grey) << "Your fleet: "
              << ColourCode(TextColour::Default);
    PrintFleetStatus(*status.homeBoard);
    std::cout << "\n";

    PrintWrappedList("   Enemy losses (" +
                     std::to_string(status.enemyLosses.size()) + "/" +
                     std::to_string(FLEET_SIZE) + "): ",
                     status.enemyLosses, 18);

    std::cout << "\n   " << ColourCode(TextColour::White)
              << (status.message.empty() ? "Awaiting orders." : status.message)
              << ColourCode(TextColour::Default) << "\n\n";

    std::cout << "   " << ColourCode(TextColour::Grey)
              << "A1-J10 fire  |  H help  |  D debug view  |  Q quit to menu"
              << ColourCode(TextColour::Default) << "\n";
}

void UI::ShowDebugScreen(const Board& computerHomeBoard,
                         const TrackingBoard& computerTrackingBoard)
{
    ClearScreen();
    std::cout << "\n";
    std::cout << "  " << ColourCode(TextColour::Red)
              << Centre("DEBUG MODE - COMPUTER GRIDS", SCREEN_WIDTH)
              << ColourCode(TextColour::Default) << "\n";
    PrintRule();
    std::cout << "\n";
    std::cout << "   Left:  the computer's ships and every shot you have landed.\n";
    std::cout << "   Right: every shot the computer has fired at your fleet.\n\n";

    const std::vector<std::string> homeLines =
        BuildBoardLines(computerHomeBoard, "COMPUTER WATERS", true);
    const std::vector<std::string> trackingLines =
        BuildTrackingLines(computerTrackingBoard, "COMPUTER SHOTS");

    PrintSideBySide(homeLines, trackingLines);

    std::cout << "\n   " << ColourCode(TextColour::Grey) << "Computer fleet: "
              << ColourCode(TextColour::Default);
    PrintFleetStatus(computerHomeBoard);
    std::cout << "\n";
}

void UI::ShowSummary(bool victory,
                     int shots,
                     int hits,
                     float accuracy,
                     char rating,
                     const std::vector<std::string>& playerLosses,
                     const std::vector<std::string>& enemyLosses)
{
    ClearScreen();
    ShowTitle();

    std::cout << "  " << ColourCode(victory ? TextColour::Green : TextColour::Red)
              << Centre(victory ? "V I C T O R Y" : "D E F E A T", SCREEN_WIDTH)
              << ColourCode(TextColour::Default) << "\n";
    std::cout << "  " << ColourCode(victory ? TextColour::Green : TextColour::Red)
              << Centre(victory ? "You destroyed the enemy fleet!"
                                : "The computer destroyed your fleet.",
                        SCREEN_WIDTH)
              << ColourCode(TextColour::Default) << "\n\n";
    PrintRule();
    std::cout << "\n";

    std::cout << "   Shots fired ......... " << shots << "\n";
    std::cout << "   Hits ................ " << hits << "\n";
    std::cout << "   Accuracy ............ " << std::fixed << std::setprecision(1)
              << accuracy << "%\n";
    std::cout << "   Performance rating .. " << rating << "\n\n";
    PrintWrappedList("   Your ships lost:     ", playerLosses, 24);
    PrintWrappedList("   Enemy ships sunk:    ", enemyLosses, 24);
    std::cout << "\n";
    PrintRule();
    std::cout << "\n";
}

std::string UI::ReadLine(const std::string& prompt, bool& endOfInput)
{
    if (!prompt.empty())
    {
        std::cout << prompt;
        std::cout.flush();
    }

    std::string line;

    if (!std::getline(std::cin, line))
    {
        endOfInput = true;
        return std::string();
    }

    return Trim(line);
}

int UI::ReadMenuChoice(const std::string& prompt, int minChoice, int maxChoice, bool& endOfInput)
{
    while (true)
    {
        const std::string text = ReadLine(prompt, endOfInput);

        if (endOfInput)
        {
            return minChoice;
        }

        if (!text.empty())
        {
            char* end = nullptr;
            const long value = std::strtol(text.c_str(), &end, 10);

            if (end != text.c_str() && *end == '\0' &&
                value >= minChoice && value <= maxChoice)
            {
                return static_cast<int>(value);
            }
        }

        std::cout << "   Invalid input. Please enter a number from "
                  << minChoice << " to " << maxChoice << ".\n";
    }
}

bool UI::ParseCoordinate(const std::string& text, int& row, int& column)
{
    std::string cleaned;

    for (std::size_t index = 0; index < text.size(); ++index)
    {
        const char character = text[index];

        if (character == ' ' || character == '\t' || character == '-')
        {
            continue;
        }

        cleaned += static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    }

    if (cleaned.size() < 2)
    {
        return false;
    }

    const char letter = cleaned[0];

    if (letter < 'A' || letter > 'J')
    {
        return false;
    }

    const std::string digits = cleaned.substr(1);

    if (digits.size() > 2)
    {
        return false;
    }

    for (std::size_t index = 0; index < digits.size(); ++index)
    {
        if (!std::isdigit(static_cast<unsigned char>(digits[index])))
        {
            return false;
        }
    }

    const int number = std::atoi(digits.c_str());

    if (number < 1 || number > BOARD_SIZE)
    {
        return false;
    }

    row = letter - 'A';
    column = number - 1;

    return true;
}

void UI::Pause(const std::string& message)
{
    std::cout << "\n   " << ColourCode(TextColour::Grey) << message
              << ColourCode(TextColour::Default) << " ";
    std::cout.flush();

    bool endOfInput = false;
    ReadLine("", endOfInput);
}

std::string UI::ToUpper(const std::string& text)
{
    std::string result = text;

    for (std::size_t index = 0; index < result.size(); ++index)
    {
        result[index] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(result[index])));
    }

    return result;
}
