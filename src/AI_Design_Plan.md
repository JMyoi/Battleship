# AI Mode Design Plan — Battleship

## Context

The game currently supports local 2-player only. The goal is to add an AI opponent with three difficulty levels (Easy / Medium / Hard), with the AI placing its ships randomly and the single-player experience being confined to one screen (the human player's view).

---

## Architecture Overview

### New Enum: `GameMode`

```cpp
enum class GameMode { LocalPvP, AIEasy, AIMedium, AIHard };
```

Stored as a member in `Game`. The mode is selected in the menu before starting.

---

## Menu Changes (`Game.cpp::drawMenu()`)

Add a second selection row below ship count:

```
[ 1v1 Local ]  [ vs AI ]
                  ↓ (if vs AI selected)
[ Easy ]  [ Medium ]  [ Hard ]
```

The existing `selectedOption` pattern (click → highlight gray) is reused for each row. Add a `selectedMode` variable similar to `selectedOption`. Start button only activates when both a ship count and a mode are chosen.

---

## New GameState Values
Add two new states to the `GameState` enum:

- **`SetupAI`** — AI randomly places its ships (no human interaction, instant)
- **`AITurn`** — AI computes and fires its shot (no human input, runs animation, then transitions back)

**New full flow for AI mode:**
```
Menu → SetupP1 → SetupAI (instant) → P1Turn → P1Transition → AITurn → P1Turn → ...
```

`P2SetupTransition`, `SetupP2`, `P2Transition`, and `P2Turn` are **skipped entirely** in AI mode. `player2` is still a regular `Player` object internally — the AI just controls it programmatically.

---

## AI Ship Placement (`placeAIShipsRandomly()`)

New method on `Game` (or `Player`). For each ship in `player2.ships`:
1. Pick a random `Direction` (Horizontal / Vertical)
2. Pick a random `row` and `col` within bounds
3. Call `player2.playerBoard.HandlePlaceShip(row, col, ship, direction)`
4. If placement fails (collision or out of bounds), retry with new random values
5. Call `ship.setShip(positions)` on success

This runs during the `SetupAI` state and transitions immediately to `P1Turn`.

---

## AI Firing Logic (`computeAIShot()`)

New method on `Game`, returns `pair<int,int>` (row, col).

### Easy
- Maintains a flat list (or scans `player1.playerBoard.grid`) for tiles not yet `Hit` or `Miss`
- Picks one at random from unfired tiles

### Medium
- Two sub-modes: **Hunt** (random) and **Target** (directed)
- State tracked in `Game`:
  - `vector<pair<int,int>> aiHuntQueue` — adjacent tiles to try after a hit
  - `bool aiHunting` — whether currently in target mode
- Logic:
  - If `aiHuntQueue` is empty → pick random unfired tile (Hunt mode)
  - On a **hit**: push all four orthogonal neighbors (that are unfired and in bounds) into `aiHuntQueue`; set `aiHunting = true`
  - If the hit caused a **sunk ship**: clear `aiHuntQueue`, set `aiHunting = false`
  - On a **miss** while hunting: pop next from `aiHuntQueue` and continue

### Hard
- Scans `player1.playerBoard.grid` directly
- Picks any tile with `TileState::Ship` and fires there (guaranteed hit every turn)

---

## AI Turn Screen (`drawAITurn()`)

New method on `Game`. Called when `state == AITurn`.

1. Calls `computeAIShot()` to get `(row, col)`
2. Calls `player1.playerBoard.HandleFire(row, col)` to register the shot and trigger animation
3. Registers the hit on the matching ship in `player1.ships` (same logic as `drawTrackingBoard`)
4. Waits for animation (`IsShotAnimationActive()`) before transitioning
5. Displays a brief overlay: `"Enemy fires at [col][row]!"` during animation
6. After animation completes → `checkGameOver()` → either `GameOver` or back to `P1Transition`

The human player still sees only their own board layout (My Board side), showing the incoming hit animation — **no new screen needed**, just the My Board view with the animation playing.

---

## Tracking Fired Tiles (Easy & Medium)

Add to `Game`:
```cpp
set<pair<int,int>> aiFiredAt;
```
Reset at game start. Updated after each `computeAIShot()` call. Used to avoid re-firing the same tile.

(Hard mode doesn't need this since it reads `TileState::Ship` directly.)

---

## Files to Modify

| File | Changes |
|------|---------|
| `src/Game.hpp` | Add `GameMode` enum, `GameMode gameMode`, AI state members (`aiHuntQueue`, `aiFiredAt`, `aiHunting`), new method declarations |
| `src/Game.cpp` | Extend `drawMenu()` for mode/difficulty selection; add `placeAIShipsRandomly()`, `computeAIShot()`, `drawAITurn()`; update state machine to branch on `gameMode` |
| `src/main.cpp` | Add `AITurn` and `SetupAI` to the `switch` dispatch |
| `src/Board.hpp/cpp` | Possibly expose a direct `fireAt(row, col)` overload to allow non-click-based firing for the AI (or reuse `HandleFire` with a position parameter) |

`Player.hpp / Player.cpp` — **no changes needed**; `player2` remains a regular `Player`, just driven by AI logic in `Game`.

---

## Key Design Principles

- **No new Player subclass** — AI logic lives in `Game` since `Game` already owns both players and the state machine. This avoids restructuring the entire Player class hierarchy.
- **Reuse existing `Player` and `Board` methods** — `HandlePlaceShip`, `HandleFire`, `checkGameOver`, ship hit registration all stay the same.
- **Single screen** — AI mode never renders a "Player 2 Turn" screen. `P2Turn`, `P2Transition`, and `P2SetupTransition` states are bypassed.
- **`Board::HandleFire` may need a position overload** — currently relies on mouse click detection (`isClicked()`). The AI needs to fire at a known `(row, col)` without a mouse event. A `HandleFireAt(int row, int col)` overload should be added to `Board`.

---

## Verification Plan

1. **Menu**: Confirm mode/difficulty buttons are selectable, start button requires both ship count and mode
2. **AI Setup**: In any AI mode, skip P2 setup screen; verify AI ships are placed legally (no overlaps, in bounds)
3. **Easy AI**: After many turns, confirm AI never fires the same cell twice; confirm shots are random
4. **Medium AI**: After a hit, confirm AI targets adjacent cells until ship sunk, then returns to random
5. **Hard AI**: Confirm every AI shot hits a ship
6. **Game Over**: Confirm game ends correctly whether human or AI sinks the last ship
7. **Single Screen**: Confirm P2Turn/P2Transition screens never appear in AI modes
