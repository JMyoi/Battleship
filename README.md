# Battleship


A fully graphical implementation of the classic **Battleship** game built in **C++** using the **Raylib** library. Two players compete on a 10×10 grid, placing ships and taking turns firing at each other's boards until one fleet is completely destroyed. The game features a Local 1V1 mode (played on the same device) and a complete single-player mode with an AI opponent across three difficulty levels, a polished graphical interface with hit/miss animations, sound effects, and a clean state-driven game flow.

---

## Demo

Main Menu
<img width="1144" height="802" alt="image" src="https://github.com/user-attachments/assets/233b1141-0ef0-47c5-897c-093a1286d9f5" />
Game Play 
<img width="1143" height="801" alt="image" src="https://github.com/user-attachments/assets/fc6548b8-7c5e-4a15-ac54-8f249121aa0a" />
<img width="1141" height="737" alt="image" src="https://github.com/user-attachments/assets/c657437b-a031-4d63-8827-109d6714313b" />
Game Over
<img width="1143" height="782" alt="image" src="https://github.com/user-attachments/assets/8faff1ec-047a-4032-bfac-7c6ad61dc7b7" />


---

## Features

- **Two game modes** — Local 1v1 multiplayer and single-player vs. AI
- **Three AI difficulty levels** — Easy, Medium, and Hard
- **Configurable fleet size** — Choose 1 to 5 ships before each match
- **Interactive ship placement** — Click to select ships, press `R` to rotate, drag onto the board
- **Turn-based gameplay** — Hit/miss detection, ship sinking, and winner announcement
- **Animations & sound** — Explosion and splash animations on every shot, sound effects on hits and sinks
- **Single-screen AI mode** — No second-player screen; the game stays on Player 1's view throughout

---

## System Design

### Architecture Overview

The project follows a layered, modular architecture that cleanly separates game logic from rendering. All classes communicate through well-defined interfaces, with the `Game` class acting as the central coordinator.

<img width="447" height="381" alt="image" src="https://github.com/user-attachments/assets/cd6ff0d2-2a1c-412e-829f-988b82d8ee51" />


### Class Responsibilities

| Class | Responsibility |
|-------|---------------|
| `Game` | Owns both players, drives the state machine, handles all AI logic, manages menu and transitions |
| `Player` | Manages a player's board and ships; handles setup, drawing, firing, and animation gating |
| `Board` | Stores the 10×10 tile grid; handles ship placement validation, fire registration, and animation state |
| `Ship` | Tracks a single ship's size, grid positions, hit segments, and sunk status |

### Game State Machine

The game progresses through a strict sequence of states managed by the `GameState` enum:

```
Menu
 └─► SetupP1
      ├─► (1v1) P2SetupTransition ──► SetupP2 ──► P1Transition
      └─► (AI)  P1Transition  ◄─────────────────────────────────┐
                   └─► TurnP1                                    │
                         ├─► (1v1) P2Transition ──► TurnP2 ──►──┘
                         ├─► (AI)  AITurn ──────────────────►───┘
                         └─► GameOver
```

In Bot mode, `P2SetupTransition`, `SetupP2`, `P2Transition`, and `TurnP2` are bypassed entirely — the human player never leaves their own screen.

---

## Bot Design

The Bot opponent is integrated directly into the `Game` class and controls `player2` programmatically, requiring no changes to the `Player` or `Board` classes.

### Ship Placement
At game start, the AI instantly places all ships using randomized positions and orientations, retrying automatically on any collision or out-of-bounds result.

### Difficulty Levels

**Easy**
Fires at a completely random unfired cell each turn. Every cell is equally likely. A running set of already-fired positions prevents repeated shots.

**Medium**
Implements a classic **Hunt / Target** algorithm that mirrors how a skilled human plays:
- **Hunt phase** — fires randomly until a ship is hit
- **Target phase** — on a hit, probes the four orthogonal neighbors to find the ship's axis
- **Lock phase** — once two consecutive hits establish a direction, the AI extends along that axis until the ship sinks or the board edge is reached; if it overshoots, it reverses and extends from the other end
- Returns to Hunt phase once a ship is fully sunk

**Hard**
Reads the player's board state directly and fires at a ship tile every single turn — guaranteed to hit on every shot with no randomness.

---

## How to Build & Run

### Requirements

- C++ compiler (`g++` on Windows/Linux, `clang++` on macOS)
- [Raylib 5.0](https://www.raylib.com/) installed
- `make` (macOS/Linux) or `mingw32-make` (Windows via w64devkit)

### macOS

```bash
git clone https://github.com/JMyoi/Battleship.git
cd Battleship
make PROJECT_NAME=game OBJS="src/*.cpp"
./game
```

### Windows (w64devkit)

```bash
git clone https://github.com/JMyoi/Battleship.git
cd Battleship
mingw32-make RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=game OBJS="src/*.cpp"
game.exe
```

### Linux

```bash
git clone https://github.com/JMyoi/Battleship.git
cd Battleship
make PROJECT_NAME=game OBJS="src/*.cpp" RAYLIB_LIBTYPE=SHARED
./game
```
