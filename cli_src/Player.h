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
    explicit Player(int shipCount = 1);

    Board& board();
    const Board& board() const;

    TrackingBoard& tracking();
    const TrackingBoard& tracking() const;

    std::vector<Ship>& ships();
    const std::vector<Ship>& ships() const;

    // Fire at enemy board; update tracking board based on result
    ShotResult attack(Player& enemy, Position target);

    // True when all ships are sunk
    bool hasLost() const;

private:
    Board m_board;
    TrackingBoard m_tracking;
    std::vector<Ship> m_ships;
};
