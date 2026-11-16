//==============================================================================================
//  MEDIA DESIGN SCHOOL AT STRAYER - DEPARTMENT OF GAME DEVELOPMENT
//  GD1P01 - Introduction to Software Engineering | Final Project
//
//  File:        Game.cpp
//  Author:      URBANOZ
//  Date:        09/11/2026
//  Description: Implements the flow of a complete game of Battleships.
//==============================================================================================

#include "Game.h"

#include <iostream>

#include "UI.h"

Game::Game()
    : m_running(true),
      m_debugMode(false),
      m_endOfInput(false),
      m_turnNumber(1),
      m_message(),
      m_playerLosses(),
      m_enemyLosses(),
      m_player(),
      m_computer()
{
}

void Game::Run()
{
    UI::Initialise();

    while (m_running && !m_endOfInput)
    {
        UI::ClearScreen();
        UI::ShowMainMenu(m_debugMode);

        const int choice = UI::ReadMenuChoice("   Select an option: ", 1, 4, m_endOfInput);

        if (m_endOfInput)
        {
            m_running = false;
            break;
        }

        switch (choice)
        {
            case 1:
                StartNewGame();
                break;

            case 2:
                UI::ShowInstructions();
                UI::Pause();
                break;

            case 3:
                m_debugMode = !m_debugMode;
                break;

            case 4:
                m_running = false;
                break;

            default:
                break;
        }
    }

    UI::ClearScreen();
    std::cout << "\n   Thank you for playing Battleships!\n";
    UI::Pause("Press Enter to close the game...");
}

void Game::StartNewGame()
{
    bool playAgain = true;

    while (playAgain && m_running && !m_endOfInput)
    {
        m_player = std::make_unique<HumanPlayer>();
        m_computer = std::make_unique<AIPlayer>();
        m_playerLosses.clear();
        m_enemyLosses.clear();
        m_message.clear();
        m_turnNumber = 1;

        if (!SetupFleets())
        {
            return;
        }

        const BattleOutcome outcome = Battle();

        if (outcome == BattleOutcome::QuitToMenu)
        {
            return;
        }

        playAgain = FinishGame(outcome == BattleOutcome::PlayerVictory);
    }
}

bool Game::SetupFleets()
{
    while (true)
    {
        if (m_endOfInput)
        {
            return false;
        }

        UI::ClearScreen();
        UI::ShowTitle();
        std::cout << "   FLEET DEPLOYMENT\n\n";
        std::cout << "     1.   Random placement (the computer positions your fleet)\n";
        std::cout << "     2.   Manual placement (you position every ship)\n\n";

        const int choice = UI::ReadMenuChoice("   Choose placement method (1-2): ",
                                              1, 2, m_endOfInput);

        if (m_endOfInput)
        {
            return false;
        }

        if (choice == 1)
        {
            m_player->GetHomeBoard().PlaceFleetRandomly();
        }
        else if (!ManualPlacement())
        {
            return false;
        }

        UI::ShowPlacementScreen(m_player->GetHomeBoard(),
                                "YOUR FLEET IS READY",
                                "Review the layout, then accept it or deploy again.");
        std::cout << "     1.   Accept this fleet and start the battle\n";
        std::cout << "     2.   Deploy the fleet again\n\n";

        const int confirmation = UI::ReadMenuChoice("   Choose (1-2): ", 1, 2, m_endOfInput);

        if (m_endOfInput)
        {
            return false;
        }

        if (confirmation == 1)
        {
            break;
        }
    }

    m_computer->GetHomeBoard().PlaceFleetRandomly();
    m_message = "Your fleet is deployed. Sink all five enemy ships!";

    return true;
}

bool Game::ManualPlacement()
{
    Board& board = m_player->GetHomeBoard();
    board.Reset();

    const std::string names[FLEET_SIZE] =
    {
        "Aircraft Carrier",
        "Battleship",
        "Destroyer",
        "Submarine",
        "Patrol Boat"
    };

    const char symbols[FLEET_SIZE] = { 'A', 'B', 'D', 'S', 'P' };
    const int lengths[FLEET_SIZE] = { 5, 4, 3, 3, 2 };

    for (int index = 0; index < FLEET_SIZE; ++index)
    {
        bool placed = false;
        std::string message = "Place your " + names[index] +
                              " (length " + std::to_string(lengths[index]) + ").";

        while (!placed)
        {
            if (m_endOfInput)
            {
                return false;
            }

            UI::ShowPlacementScreen(board, "MANUAL FLEET DEPLOYMENT", message);
            std::cout << "   Ship " << (index + 1) << " of " << FLEET_SIZE << ": "
                      << names[index] << " (" << lengths[index] << " squares)\n\n";

            const std::string orientationText = UI::ToUpper(
                UI::ReadLine("   Orientation (H = across, V = down): ", m_endOfInput));

            if (m_endOfInput)
            {
                return false;
            }

            if (orientationText == "Q")
            {
                return false;
            }

            if (orientationText != "H" && orientationText != "V")
            {
                message = "Please type H for horizontal or V for vertical.";
                continue;
            }

            const Orientation orientation = (orientationText == "H")
                                            ? Orientation::Horizontal
                                            : Orientation::Vertical;

            const std::string coordinateText = UI::ReadLine(
                "   Starting square (e.g. A5): ", m_endOfInput);

            if (m_endOfInput)
            {
                return false;
            }

            if (UI::ToUpper(coordinateText) == "Q")
            {
                return false;
            }

            int row = 0;
            int column = 0;

            if (!UI::ParseCoordinate(coordinateText, row, column))
            {
                message = "Invalid square. Use a letter A-J and a number 1-10.";
                continue;
            }

            const Ship candidate(names[index], symbols[index], lengths[index]);

            if (!board.CanPlaceShip(candidate, row, column, orientation))
            {
                if (orientation == Orientation::Horizontal &&
                    column + lengths[index] > BOARD_SIZE)
                {
                    message = "That ship would run off the right edge. Try another square.";
                }
                else if (orientation == Orientation::Vertical &&
                         row + lengths[index] > BOARD_SIZE)
                {
                    message = "That ship would run off the bottom edge. Try another square.";
                }
                else
                {
                    message = "That square overlaps another ship. Try another square.";
                }

                continue;
            }

            board.PlaceShip(candidate, row, column, orientation);
            placed = true;
        }
    }

    return true;
}

Game::BattleOutcome Game::Battle()
{
    bool playerTurn = true;

    while (m_running && !m_endOfInput)
    {
        if (playerTurn)
        {
            if (!HumanTurn())
            {
                return BattleOutcome::QuitToMenu;
            }

            if (m_computer->GetHomeBoard().IsFleetDestroyed())
            {
                return BattleOutcome::PlayerVictory;
            }
        }
        else
        {
            ComputerTurn();

            if (m_player->GetHomeBoard().IsFleetDestroyed())
            {
                return BattleOutcome::ComputerVictory;
            }
        }

        playerTurn = !playerTurn;

        if (playerTurn)
        {
            ++m_turnNumber;
        }
    }

    return BattleOutcome::QuitToMenu;
}

void Game::RedrawBattle()
{
    BattleStatus status;
    status.homeBoard = &m_player->GetHomeBoard();
    status.trackingBoard = &m_player->GetTrackingBoard();
    status.turnNumber = m_turnNumber;
    status.shotsFired = m_player->GetShotsFired();
    status.hits = m_player->GetHits();
    status.shipsRemaining = m_computer->GetHomeBoard().GetShipsAfloat();
    status.debugMode = m_debugMode;
    status.message = m_message;
    status.playerLosses = m_playerLosses;
    status.enemyLosses = m_enemyLosses;

    UI::ShowBattleScreen(status);
}

bool Game::HumanTurn()
{
    // While Debug Mode is on, show both computer grids at the start of each turn.
    if (m_debugMode)
    {
        ShowDebugView();
    }

    while (m_running && !m_endOfInput)
    {
        RedrawBattle();

        const std::string input = UI::ReadLine("   Fire at (A1-J10): ", m_endOfInput);

        if (m_endOfInput)
        {
            return false;
        }

        const std::string command = UI::ToUpper(input);

        if (command.empty())
        {
            m_message = "Type a square such as B7, or H for help.";
            continue;
        }

        if (command == "Q")
        {
            if (ConfirmQuit())
            {
                return false;
            }

            m_message = "The battle continues.";
            continue;
        }

        if (command == "H")
        {
            UI::ShowInstructions();
            UI::Pause();
            m_message = "Choose a target square.";
            continue;
        }

        if (command == "D")
        {
            if (!m_debugMode)
            {
                m_debugMode = true;
            }

            ShowDebugView();
            continue;
        }

        int row = 0;
        int column = 0;

        if (!UI::ParseCoordinate(input, row, column))
        {
            m_message = "Invalid square. Use a letter A-J and a number 1-10 (for example C7).";
            continue;
        }

        if (m_player->GetTrackingBoard().IsTargeted(row, column))
        {
            m_message = "You have already fired at that square. Choose another.";
            continue;
        }

        std::string shipName;
        const ShotResult result = m_computer->GetHomeBoard().ReceiveShot(row, column, shipName);

        if (result == ShotResult::Invalid)
        {
            m_message = "That square cannot be targeted. Choose another.";
            continue;
        }

        m_player->GetTrackingBoard().Record(row, column,
                                            result == ShotResult::Miss ? CELL_MISS : CELL_HIT);
        m_player->RecordShot(result != ShotResult::Miss);

        const std::string square = FormatSquare(row, column);

        if (result == ShotResult::Miss)
        {
            m_message = "MISS!  Nothing was at " + square + ".";
        }
        else if (result == ShotResult::Hit)
        {
            m_message = "HIT!  You hit the computer's " + shipName + " at " + square + ".";
        }
        else
        {
            m_message = "SUNK!  You destroyed the computer's " + shipName +
                        " at " + square + "!";
            m_enemyLosses.push_back(shipName);
        }

        RedrawBattle();

        if (!m_computer->GetHomeBoard().IsFleetDestroyed())
        {
            UI::Pause("Press Enter to continue...");
        }

        return true;
    }

    return false;
}

void Game::ComputerTurn()
{
    m_message = "The computer is choosing a target...";
    RedrawBattle();
    UI::Pause("Press Enter to let the computer fire...");

    const Position target = m_computer->ChooseTarget();

    if (!IsInsideBoard(target))
    {
        m_message = "The computer has no squares left to fire at.";
        RedrawBattle();
        UI::Pause();
        return;
    }

    std::string shipName;
    const ShotResult result = m_player->GetHomeBoard().ReceiveShot(target.row, target.column,
                                                                   shipName);

    if (result == ShotResult::Invalid)
    {
        m_message = "The computer's shot could not be resolved. Its turn is skipped.";
        RedrawBattle();
        UI::Pause();
        return;
    }

    m_computer->GetTrackingBoard().Record(target.row, target.column,
                                          result == ShotResult::Miss ? CELL_MISS : CELL_HIT);
    m_computer->RecordShot(result != ShotResult::Miss);
    m_computer->ReportResult(target, result);

    const std::string square = FormatSquare(target.row, target.column);

    if (result == ShotResult::Miss)
    {
        m_message = "The computer fired at " + square + ": MISS.";
    }
    else if (result == ShotResult::Hit)
    {
        m_message = "The computer fired at " + square + ": HIT! Your " +
                    shipName + " was damaged.";
    }
    else
    {
        m_message = "The computer fired at " + square + ": your " + shipName + " was SUNK!";
        m_playerLosses.push_back(shipName);
    }

    RedrawBattle();
    UI::Pause();
}

void Game::ShowDebugView()
{
    UI::ClearScreen();
    UI::ShowDebugScreen(m_computer->GetHomeBoard(), m_computer->GetTrackingBoard());

    const std::string answer = UI::ToUpper(UI::ReadLine(
        "   Press Enter to return, or type D to turn Debug Mode off: ", m_endOfInput));

    if (answer == "D")
    {
        m_debugMode = false;
        m_message = "Debug Mode is now OFF.";
    }
}

bool Game::FinishGame(bool victory)
{
    const int shots = m_player->GetShotsFired();
    const int hits = m_player->GetHits();
    const float accuracy = m_player->GetAccuracy();

    // TF: Variable Type - bool, char, int and float are all used to build the summary.
    const bool playerWon = victory;
    const int totalShots = shots;
    const float hitRate = accuracy;

    char rating = 'F';

    if (playerWon)
    {
        if (hitRate >= 60.0f)
        {
            rating = 'S';
        }
        else if (hitRate >= 45.0f)
        {
            rating = 'A';
        }
        else if (hitRate >= 30.0f)
        {
            rating = 'B';
        }
        else
        {
            rating = 'C';
        }
    }

    UI::ShowSummary(playerWon, totalShots, hits, hitRate, rating,
                    m_playerLosses, m_enemyLosses);

    std::cout << "     1.   Play again\n";
    std::cout << "     2.   Return to the main menu\n\n";

    const int choice = UI::ReadMenuChoice("   Choose (1-2): ", 1, 2, m_endOfInput);

    return choice == 1 && !m_endOfInput;
}

bool Game::ConfirmQuit()
{
    const std::string answer = UI::ToUpper(
        UI::ReadLine("   Quit this game and return to the main menu? (Y/N): ", m_endOfInput));

    if (m_endOfInput)
    {
        return true;
    }

    return answer == "Y" || answer == "YES";
}

std::string Game::FormatSquare(int row, int column) const
{
    std::string square;
    square += static_cast<char>('A' + row);
    square += std::to_string(column + 1);

    return square;
}
