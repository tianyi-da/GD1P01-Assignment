# Battleships

**GD1P01 - Introduction to Software Engineering | Final Project**  
Student: **URBANOZ**  
Language: **C++17** · IDE: **Visual Studio 2022**

A single-player Battleship game for Windows, played in a console window against
a computer opponent. This repository contains the Visual Studio solution and
source code only - no build output or compiled binaries.

---

## Run in Visual Studio 2022

1. Open **`Battleships.sln`**.
2. Select **Debug** or **Release** and **x64** in the toolbar.
3. **Build > Build Solution** (`Ctrl+Shift+B`).
4. **Debug > Start Without Debugging** (`Ctrl+F5`).

Battleships is a **console application**. Visual Studio opens a terminal window
and the game runs there; there is no separate GUI. All input is typed into that
terminal window.

You can also run the built executable from a terminal:

```
bin\Release\x64\Battleships.exe
```

The path is created by Visual Studio when you build. Nothing under `bin\` or
`obj\` is committed to this repository.

---

## Controls

| Screen | Key | Action |
|---|---|---|
| Main menu | `1` | New Game |
| Main menu | `2` | Instructions |
| Main menu | `3` | Debug Mode ON / OFF |
| Main menu | `4` | Exit |
| Deployment | `1` | Random placement |
| Deployment | `2` | Manual placement (`H`/`V`, then a square such as `A5`) |
| Battle | `A1` - `J10` | Fire at that square |
| Battle | `H` | Help |
| Battle | `D` | Debug view of both computer grids |
| Battle | `Q` | Quit to the main menu |
| Game over | `1` / `2` | Play again / return to main menu |

---

## Rules implemented

* 10 x 10 boards with five ships: Carrier (5), Battleship (4), Destroyer (3),
  Submarine (3), Patrol Boat (2).
* Two grids per player: your waters (ships and incoming shots) and the enemy
  waters tracking grid.
* Random or manual fleet deployment with validation for bounds, overlap,
  coordinates and duplicate shots.
* Turn-based play with hit / miss / sunk announcements and a shot log.
* A functional computer opponent: checkerboard hunt plus neighbour targeting.
* Victory / Defeat summary with shots, hits, accuracy, rating and ships lost.
* Debug Mode showing both computer grids, available from the main menu and with
  the `D` key.
* New games can be started without leaving the program.

---

## Project layout

```
Battleships/
├── Battleships.sln
├── Battleships.vcxproj
├── Battleships.vcxproj.filters
├── main.cpp
├── Game.h / Game.cpp
├── Board.h / Board.cpp
├── Ship.h / Ship.cpp
├── TrackingBoard.h / TrackingBoard.cpp
├── Player.h / Player.cpp
├── HumanPlayer.h / HumanPlayer.cpp
├── AIPlayer.h / AIPlayer.cpp
├── UI.h / UI.cpp
├── Constants.h
├── Position.h
├── README.md
└── .gitignore
```

Build output is generated in `bin\<Configuration>\<Platform>\` and
`obj\<Configuration>\<Platform>\`; both are ignored by Git.

---

## Technical features

Every required C++ feature is marked once in the source with a `// TF:` comment.

| Feature | File | Line |
|---|---|---|
| Variable types | Game.cpp | 517 |
| `const` keyword | Constants.h | 14 |
| Logical operators | Board.cpp | 61 |
| Arithmetic operators | Ship.cpp | 165 |
| Relational operators | Ship.cpp | 188 |
| Iteration structures | Board.cpp | 112 |
| Conditional statements | Board.cpp | 198 |
| Arrays | Board.h | 50 |
| Default parameters | UI.h | 61 |
| Pseudo random numbers | Board.cpp | 121 |
| Pointers initialised | Ship.h | 60 |
| Pointers dereferenced | Ship.cpp | 175 |
| References | Board.cpp | 87 |
| Dynamic memory | Ship.cpp | 39 |
| Classes | Ship.h | 23 |
| Objects | main.cpp | 22 |
| Constructor | Ship.cpp | 28 |
| Destructor | Ship.cpp | 105 |
| Copy constructor | Ship.cpp | 48 |
| Public / private access | Ship.h | 50 |

The dynamic hit map is allocated in `Ship`'s constructor, deep-copied by the
copy constructor and released by the destructor. Ships are stored in a
`std::vector`, copied into a `Board` through a reference parameter, and random
numbers drive fleet placement. The 2D array is the actual board.

---

## Build quality

* Visual Studio 2022 / MSVC 14.44: Debug and Release, x64 and Win32, all build
  with **0 warnings and 0 errors** at Warning Level 3.
* GCC 16.2: clean with `-std=c++17 -Wall -Wextra -Wpedantic -Wconversion
  -Wshadow -Werror`.
* 3,000 random fleet placements and 500 complete AI games tested.
* Full games played to both Victory and Defeat, including manual deployment,
  Debug Mode, Help, Quit and Play Again.
