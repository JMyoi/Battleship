#pragma once
#include "Player.h"

enum class GameState { SetupP1, SetupP2, TurnP1, TurnP2, GameOver };

/**
 * @brief Controls setup + turn switching + win condition.
 * No UI inside; CLI/UI calls these methods.
 */
class Game {
public:
    explicit Game(int shipCount);

    GameState state() const;
    int currentPlayerIndex() const; // 0 or 1
    int winnerIndex() const;        // valid when GameOver

    Player& player(int idx);
    const Player& player(int idx) const;

    // Place the next ship for the current setup player (size depends on index)
    bool placeNextShip(Position start, Direction dir);

    // Current player fires at opponent
    ShotResult fire(Position target);

    bool isOver() const;

private:
    int shipSizeForIndex(int i) const { return i + 1; }

    void advanceSetup();
    void advanceTurn();
    void checkGameOver();

    int m_shipCount;
    GameState m_state;
    int m_nextShipToPlace; // 0..shipCount-1 within current setup phase
    int m_winner;

    Player m_p1;
    Player m_p2;
};
