//
// Bachelor of Software Engineering
// Media Design School at Strayer
// Auckland
// New Zealand
//
// (c) 2026 Media Design School at Strayer
//
// File Name    : Game.cpp
// Description  : Game controller implementation
// Author       : Your Name
// Mail         : your.name@mds.ac.nz
//

#include "Game.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cctype>

namespace
{
void ClearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string ToUpper(std::string text)
{
    for (int i = 0; i < static_cast<int>(text.size()); ++i)
    {
        text[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(text[i])));
    }
    return text;
}

// Removes all whitespace so "b 7", " B7 " and "B7" are treated identically.
std::string StripSpaces(const std::string& text)
{
    std::string result;
    for (int i = 0; i < static_cast<int>(text.size()); ++i)
    {
        if (!std::isspace(static_cast<unsigned char>(text[i])))
        {
            result += text[i];
        }
    }
    return result;
}

std::string ReadLine(const std::string& prompt)
{
    std::cout << prompt;
    std::string input;
    if (!std::getline(std::cin, input))
    {
        std::cout << "\nInput closed. Exiting game.\n";
        std::exit(0);
    }
    return input;
}

void Pause()
{
    ReadLine("\nPress Enter to continue...");
}

bool AskYesNo(const std::string& prompt)
{
    while (true)
    {
        const std::string answer = ToUpper(StripSpaces(ReadLine(prompt)));
        if (answer == "Y" || answer == "YES")
        {
            return true;
        }
        if (answer == "N" || answer == "NO")
        {
            return false;
        }
        std::cout << "  Please answer Y or N.\n";
    }
}

// Accepts a letter A-J followed by a number 1-10 (e.g. "B7", "j10"). Letter = column, number = row.
bool ParseCoordinate(const std::string& input, int& row, int& col)
{
    const std::string text = ToUpper(StripSpaces(input));
    const int length = static_cast<int>(text.size());
    if (length < 2 || length > 3)
    {
        return false;
    }
    const char letter = text[0];
    if (letter < 'A' || letter >= 'A' + BOARD_SIZE)
    {
        return false;
    }
    int number = 0;
    for (int i = 1; i < length; ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(text[i])))
        {
            return false;
        }
        number = number * 10 + (text[i] - '0');
    }
    if (number < 1 || number > BOARD_SIZE)
    {
        return false;
    }
    col = letter - 'A';
    row = number - 1;
    return true;
}

std::string CoordToString(int row, int col)
{
    return std::string(1, static_cast<char>('A' + col)) + std::to_string(row + 1);
}

std::string Pad(const std::string& text, int width)
{
    std::string result = text;
    for (int i = static_cast<int>(text.size()); i < width; ++i)
    {
        result += ' ';
    }
    return result;
}
}

Game::Game()
    : m_playerBoard(nullptr), m_aiBoard(nullptr), m_debugMode(false),
      m_turns(0), m_playerShots(0), m_playerHits(0), m_aiShots(0), m_aiHits(0),
      m_targetCount(0)
{
}

Game::~Game()
{
    delete m_playerBoard;
    delete m_aiBoard;
}

void Game::Run()
{
    bool exitRequested = false;
    while (!exitRequested)
    {
        ShowMainMenu();
        const std::string choice = StripSpaces(ReadLine("  Select an option (1-4): "));

        if (choice == "1")
        {
            PlayGame();
        }
        else if (choice == "2")
        {
            ShowInstructions();
        }
        else if (choice == "3")
        {
            m_debugMode = !m_debugMode;
            m_menuMessage = std::string("Debug Mode is now ") + (m_debugMode ? "ON" : "OFF") + ".";
        }
        else if (choice == "4")
        {
            exitRequested = AskYesNo("  Are you sure you want to exit? (Y/N): ");
        }
        else
        {
            m_menuMessage = "Invalid option. Please type 1, 2, 3 or 4.";
        }
    }
    ClearScreen();
    std::cout << "\n  Thanks for playing BATTLESHIPS. Fair winds, Admiral!\n\n";
}

void Game::ShowMainMenu() const
{
    ClearScreen();
    std::cout << "\n";
    std::cout << "  ==========================================\n";
    std::cout << "   ____    _  _____ _____ _     _____ \n";
    std::cout << "  | __ )  / \\|_   _|_   _| |   | ____|\n";
    std::cout << "  |  _ \\ / _ \\ | |   | | | |   |  _|  \n";
    std::cout << "  | |_) / ___ \\| |   | | | |___| |___ \n";
    std::cout << "  |____/_/   \\_\\_|   |_| |_____|_____|\n";
    std::cout << "        S  H  I  P  S   -   v1.0\n";
    std::cout << "  ==========================================\n\n";
    std::cout << "    1. New Game  (you vs. computer)\n";
    std::cout << "    2. How to Play\n";
    std::cout << "    3. Toggle Debug Mode   [currently " << (m_debugMode ? "ON " : "OFF") << "]\n";
    std::cout << "    4. Exit\n\n";
    if (!m_menuMessage.empty())
    {
        std::cout << "  >> " << m_menuMessage << "\n\n";
    }
}

void Game::ShowInstructions() const
{
    ClearScreen();
    std::cout << "\n  ================= HOW TO PLAY =================\n\n";
    std::cout << "  GOAL   Sink all 5 enemy ships before the computer sinks yours.\n\n";
    std::cout << "  FLEET  Aircraft Carrier (5), Battleship (4), Destroyer (3),\n";
    std::cout << "         Submarine (3), Patrol Boat (2).\n\n";
    std::cout << "  SETUP  Choose 'random' to have the computer place your fleet, or\n";
    std::cout << "         'manual' to place each ship yourself: enter a start\n";
    std::cout << "         coordinate (e.g. B3) and orientation H (extends right)\n";
    std::cout << "         or V (extends down). Ships may not overlap.\n\n";
    std::cout << "  PLAY   Each turn type a coordinate: a letter A-J and a number\n";
    std::cout << "         1-10, e.g.  E5   or   j10.  You will be told if it is a\n";
    std::cout << "         HIT, a MISS, or if you SUNK a ship. The computer then fires.\n\n";
    std::cout << "  SCREEN Left grid : YOUR FLEET (ships and the enemy's shots)\n";
    std::cout << "         Right grid: YOUR SHOTS at the enemy waters\n\n";
    std::cout << "  SYMBOLS  ~ water   # your ship   X hit   o miss\n\n";
    std::cout << "  COMMANDS during battle:\n";
    std::cout << "         <coordinate>  fire at that square\n";
    std::cout << "         D             toggle Debug Mode (reveals enemy grids)\n";
    std::cout << "         Q             quit to the main menu\n";
    Pause();
}

void Game::PlayGame()
{
    m_playerBoard = new Board();
    m_aiBoard = new Board();
    m_turns = 0;
    m_playerShots = 0;
    m_playerHits = 0;
    m_aiShots = 0;
    m_aiHits = 0;
    m_targetCount = 0;
    m_menuMessage.clear();
    m_playerMsg = "The battle begins! Choose your first target.";
    m_aiMsg.clear();

    if (SetupPlayerFleet())
    {
        m_aiBoard->PlaceRandomly();

        bool quit = false;
        bool playerWon = false;
        bool finished = false;
        while (!finished)
        {
            DrawScreen();
            if (!PlayerTurn())
            {
                quit = true;
                break;
            }
            ++m_turns;
            if (m_aiBoard->AllShipsSunk())
            {
                playerWon = true;
                finished = true;
            }
            else
            {
                AiTurn();
                if (m_playerBoard->AllShipsSunk())
                {
                    playerWon = false;
                    finished = true;
                }
            }
        }
        if (!quit)
        {
            ShowSummary(playerWon);
        }
    }

    delete m_playerBoard;
    m_playerBoard = nullptr;
    delete m_aiBoard;
    m_aiBoard = nullptr;
}

bool Game::SetupPlayerFleet()
{
    std::string error;
    while (true)
    {
        ClearScreen();
        std::cout << "\n  ============== FLEET SETUP ==============\n\n";
        std::cout << "    1. Random placement (computer positions your ships)\n";
        std::cout << "    2. Manual placement (you position each ship)\n";
        std::cout << "    3. Back to main menu\n\n";
        if (!error.empty())
        {
            std::cout << "  >> " << error << "\n\n";
        }
        const std::string choice = StripSpaces(ReadLine("  Select an option (1-3): "));
        error.clear();

        if (choice == "1")
        {
            if (RandomPlacement())
            {
                return true;
            }
        }
        else if (choice == "2")
        {
            if (ManualPlacement())
            {
                return true;
            }
        }
        else if (choice == "3")
        {
            return false;
        }
        else
        {
            error = "Invalid option. Please type 1, 2 or 3.";
        }
    }
}

bool Game::RandomPlacement()
{
    while (true)
    {
        m_playerBoard->PlaceRandomly();
        ClearScreen();
        std::cout << "\n  Your randomly placed fleet:\n\n";
        DrawSingleBoard(*m_playerBoard);
        if (AskYesNo("\n  Keep this layout? (Y = keep, N = reshuffle): "))
        {
            return true;
        }
    }
}

bool Game::ManualPlacement()
{
    m_playerBoard->Reset();
    std::string error;
    for (int i = 0; i < FLEET_SIZE; ++i)
    {
        bool placed = false;
        while (!placed)
        {
            ClearScreen();
            std::cout << "\n  ======== MANUAL PLACEMENT ========\n\n";
            DrawSingleBoard(*m_playerBoard);
            const Ship& ship = m_playerBoard->GetShip(i);
            std::cout << "\n  Placing: " << ship.GetName() << " (length " << ship.GetLength()
                      << ")  [ship " << (i + 1) << " of " << FLEET_SIZE << "]\n";
            if (!error.empty())
            {
                std::cout << "  >> " << error << "\n";
                error.clear();
            }

            const std::string coordText = ReadLine("  Start coordinate (e.g. B3), or Q to cancel: ");
            if (ToUpper(StripSpaces(coordText)) == "Q")
            {
                return false;
            }
            int row = 0;
            int col = 0;
            if (!ParseCoordinate(coordText, row, col))
            {
                error = "Invalid coordinate. Use a letter A-J followed by a number 1-10.";
                continue;
            }

            const std::string orient = ToUpper(StripSpaces(
                ReadLine("  Orientation: H = horizontal (extends right), V = vertical (extends down): ")));
            if (orient != "H" && orient != "V")
            {
                error = "Invalid orientation. Please type H or V.";
                continue;
            }

            if (m_playerBoard->PlaceShip(i, row, col, orient == "H"))
            {
                placed = true;
            }
            else
            {
                error = "That ship does not fit there (off the grid or overlapping another ship).";
            }
        }
    }
    ClearScreen();
    std::cout << "\n  Your fleet is ready:\n\n";
    DrawSingleBoard(*m_playerBoard);
    Pause();
    return true;
}

void Game::DrawSingleBoard(const Board& board) const
{
    std::cout << "    " << Board::Header() << "\n";
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        std::cout << "    " << board.RowToString(row, true) << "\n";
    }
}

void Game::DrawBoardPair(const char* leftTitle, const Board& left, bool leftShips,
                         const char* rightTitle, const Board& right, bool rightShips) const
{
    std::cout << "    " << Pad(leftTitle, 29) << rightTitle << "\n";
    std::cout << "    " << Board::Header() << "      " << Board::Header() << "\n";
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        std::cout << "    " << left.RowToString(row, leftShips) << "      "
                  << right.RowToString(row, rightShips) << "\n";
    }
}

void Game::DrawScreen() const
{
    ClearScreen();
    std::cout << "  ================== BATTLESHIPS ==================";
    std::cout << (m_debugMode ? "   [DEBUG MODE ON]" : "") << "\n\n";

    DrawBoardPair("YOUR FLEET", *m_playerBoard, true,
                  "YOUR SHOTS (ENEMY WATERS)", *m_aiBoard, false);

    std::cout << "\n    Legend: ~ water   # ship   X hit   o miss\n";
    std::cout << "    Your ships afloat: " << m_playerBoard->ShipsAfloat() << "/" << FLEET_SIZE
              << "     Enemy ships afloat: " << m_aiBoard->ShipsAfloat() << "/" << FLEET_SIZE
              << "     Turn: " << (m_turns + 1) << "\n";

    if (m_debugMode)
    {
        std::cout << "\n";
        DrawBoardPair("[DEBUG] ENEMY FLEET", *m_aiBoard, true,
                      "[DEBUG] ENEMY SHOTS AT YOU", *m_playerBoard, false);
    }

    std::cout << "\n  ------------------------------------------------\n";
    if (!m_playerMsg.empty())
    {
        std::cout << "  " << m_playerMsg << "\n";
    }
    if (!m_aiMsg.empty())
    {
        std::cout << "  " << m_aiMsg << "\n";
    }
    std::cout << "  ------------------------------------------------\n";
    std::cout << "  Controls: coordinate (e.g. B7) = fire | D = debug on/off | Q = quit to menu\n\n";
}

bool Game::PlayerTurn()
{
    while (true)
    {
        const std::string input = ReadLine("  Fire at > ");
        const std::string command = ToUpper(StripSpaces(input));

        if (command == "Q")
        {
            if (AskYesNo("  Quit to the main menu? Current game will be lost (Y/N): "))
            {
                return false;
            }
            DrawScreen();
            continue;
        }
        if (command == "D")
        {
            m_debugMode = !m_debugMode;
            DrawScreen();
            continue;
        }

        int row = 0;
        int col = 0;
        if (!ParseCoordinate(command, row, col))
        {
            std::cout << "  Invalid input. Enter a letter A-J and a number 1-10 (e.g. B7), D or Q.\n";
            continue;
        }

        int sunkIndex = -1;
        const ShotResult result = m_aiBoard->Fire(row, col, sunkIndex);
        if (result == ShotResult::AlreadyShot)
        {
            std::cout << "  You already fired at " << CoordToString(row, col) << ". Pick a new target.\n";
            continue;
        }

        ++m_playerShots;
        const std::string target = CoordToString(row, col);
        if (result == ShotResult::Miss)
        {
            m_playerMsg = "You fired at " + target + ": MISS.";
        }
        else if (result == ShotResult::Hit)
        {
            ++m_playerHits;
            m_playerMsg = "You fired at " + target + ": HIT!";
        }
        else
        {
            ++m_playerHits;
            m_playerMsg = "You fired at " + target + ": HIT! You SUNK the enemy "
                          + m_aiBoard->GetShip(sunkIndex).GetName() + "!";
        }
        return true;
    }
}

void Game::QueueNeighbours(int row, int col)
{
    const int dRow[4] = { -1, 1, 0, 0 };
    const int dCol[4] = { 0, 0, -1, 1 };
    for (int i = 0; i < 4; ++i)
    {
        const int r = row + dRow[i];
        const int c = col + dCol[i];
        if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && m_targetCount < BOARD_SIZE * BOARD_SIZE)
        {
            m_targetQueue[m_targetCount][0] = r;
            m_targetQueue[m_targetCount][1] = c;
            ++m_targetCount;
        }
    }
}

void Game::ChooseAiTarget(int& row, int& col)
{
    // Target mode: follow up around previous hits.
    while (m_targetCount > 0)
    {
        --m_targetCount;
        const int r = m_targetQueue[m_targetCount][0];
        const int c = m_targetQueue[m_targetCount][1];
        if (!m_playerBoard->HasBeenShot(r, c))
        {
            row = r;
            col = c;
            return;
        }
    }

    // Hunt mode: prefer a checkerboard pattern, fall back to any unshot square.
    int candidates[BOARD_SIZE * BOARD_SIZE][2];
    int count = 0;
    for (int pass = 0; pass < 2 && count == 0; ++pass)
    {
        for (int r = 0; r < BOARD_SIZE; ++r)
        {
            for (int c = 0; c < BOARD_SIZE; ++c)
            {
                const bool parityOk = (pass == 1) || ((r + c) % 2 == 0);
                if (parityOk && !m_playerBoard->HasBeenShot(r, c))
                {
                    candidates[count][0] = r;
                    candidates[count][1] = c;
                    ++count;
                }
            }
        }
    }
    const int pick = rand() % count;
    row = candidates[pick][0];
    col = candidates[pick][1];
}

void Game::AiTurn()
{
    int row = 0;
    int col = 0;
    ChooseAiTarget(row, col);

    int sunkIndex = -1;
    const ShotResult result = m_playerBoard->Fire(row, col, sunkIndex);
    ++m_aiShots;
    const std::string target = CoordToString(row, col);

    if (result == ShotResult::Miss)
    {
        m_aiMsg = "Computer fired at " + target + ": MISS.";
    }
    else if (result == ShotResult::Hit)
    {
        ++m_aiHits;
        QueueNeighbours(row, col);
        m_aiMsg = "Computer fired at " + target + ": HIT!";
    }
    else if (result == ShotResult::Sunk)
    {
        ++m_aiHits;
        m_targetCount = 0;
        m_aiMsg = "Computer fired at " + target + ": HIT! It SUNK your "
                  + m_playerBoard->GetShip(sunkIndex).GetName() + "!";
    }
}

void Game::ShowSummary(bool playerWon) const
{
    ClearScreen();
    std::cout << "\n  ==================================================\n";
    if (playerWon)
    {
        std::cout << "                 *** V I C T O R Y ***\n";
        std::cout << "        You sank the entire enemy fleet, Admiral!\n";
    }
    else
    {
        std::cout << "                  *** D E F E A T ***\n";
        std::cout << "         The computer sank your entire fleet.\n";
    }
    std::cout << "  ==================================================\n\n";

    // TF: Variable Type
    bool hasShots = m_playerShots > 0;
    int turnsTaken = m_turns;
    float accuracy = 0.0f;
    char rank = 'C';

    if (hasShots)
    {
        // TF: Arithmetic Operator
        accuracy = (static_cast<float>(m_playerHits) / static_cast<float>(m_playerShots)) * 100.0f;
    }
    if (accuracy >= 60.0f)
    {
        rank = 'S';
    }
    else if (accuracy >= 45.0f)
    {
        rank = 'A';
    }
    else if (accuracy >= 30.0f)
    {
        rank = 'B';
    }

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  GAME SUMMARY\n";
    std::cout << "  Status           : " << (playerWon ? "Victory" : "Defeat") << "\n";
    std::cout << "  Turns played     : " << turnsTaken << "\n";
    std::cout << "  Your shots       : " << m_playerShots << " (" << m_playerHits << " hits)\n";
    std::cout << "  Your accuracy    : " << accuracy << "%   Rank: " << rank << "\n";
    std::cout << "  Computer shots   : " << m_aiShots << " (" << m_aiHits << " hits)\n";
    std::cout << "  Your ships left  : " << m_playerBoard->ShipsAfloat() << "/" << FLEET_SIZE << "\n";
    std::cout << "  Enemy ships left : " << m_aiBoard->ShipsAfloat() << "/" << FLEET_SIZE << "\n\n";

    DrawBoardPair("FINAL: YOUR FLEET", *m_playerBoard, true,
                  "FINAL: ENEMY FLEET REVEALED", *m_aiBoard, true);
    Pause();
}
