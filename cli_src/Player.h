#pragma once
#include <vector>
#include "Board.h"
#include "Ship.h"
#include "TrackingBoard.h"

/**
 * @brief A player owns a board + ships, and keeps a tracking board for shots fired.
 */
class Player {
public:
    explicit Player(int shipCount = 1,
                    PlayerType type = PlayerType::Human,
                    AIDifficulty difficulty = AIDifficulty::None);

    Board& board();
    const Board& board() const;

    TrackingBoard& tracking();
    const TrackingBoard& tracking() const;

    std::vector<Ship>& ships();
    const std::vector<Ship>& ships() const;

    ShotResult attack(Player& enemy, Position target);
    bool hasLost() const;

    bool isAI() const;
    PlayerType type() const;
    AIDifficulty difficulty() const;

    Position chooseAITarget(const Player& enemy);
    void updateAIStateAfterShot(Position target, ShotResult result);
    bool placeAllShipsRandomly();

private:
    bool isValidPosition(Position p) const;
    bool isUntriedPosition(Position p) const;
    void addMediumNeighbors(Position p);

    Board m_board;
    TrackingBoard m_tracking;
    std::vector<Ship> m_ships;

    PlayerType m_type;
    AIDifficulty m_difficulty;
    std::vector<Position> m_mediumTargets;
};