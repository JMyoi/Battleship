#pragma once
#include "Player.h"

enum class GameState { SetupP1, SetupP2, TurnP1, TurnP2, GameOver };

/**
 * @brief Controls setup + turn switching + win condition.
 * No UI inside; CLI/UI calls these methods.
 */
class Game {
public:
    explicit Game(int shipCount,
                  PlayerType p1Type = PlayerType::Human,
                  PlayerType p2Type = PlayerType::Human,
                  AIDifficulty p1Difficulty = AIDifficulty::None,
                  AIDifficulty p2Difficulty = AIDifficulty::None);

    GameState state() const;
    int currentPlayerIndex() const; // 0 or 1
    int winnerIndex() const;        // valid when GameOver

    Player& player(int idx);
    const Player& player(int idx) const;
    bool placeNextShip(Position start, Direction dir);
    ShotResult fire(Position target);

    bool currentPlayerIsAI() const;
    ShotResult fireAITurn();

    bool isOver() const;
    bool autoPlaceShipsForCurrentPlayer();

private:
    int shipSizeForIndex(int i) const { return i + 1; }

    void advanceSetup();
    void advanceTurn();
    void checkGameOver();

    int m_shipCount;
    GameState m_state;
    int m_nextShipToPlace;
    int m_winner;

    Player m_p1;
    Player m_p2;
};