#include "Player.h"

Player::Player(int shipCount) {
    if (shipCount < 1) shipCount = 1;
    if (shipCount > 5) shipCount = 5;

    m_ships.reserve(shipCount);
    for (int i = 0; i < shipCount; ++i) {
        m_ships.emplace_back(i + 1); // sizes 1..shipCount
    }
}

Board& Player::board() { return m_board; }
const Board& Player::board() const { return m_board; }

TrackingBoard& Player::tracking() { return m_tracking; }
const TrackingBoard& Player::tracking() const { return m_tracking; }

std::vector<Ship>& Player::ships() { return m_ships; }
const std::vector<Ship>& Player::ships() const { return m_ships; }

ShotResult Player::attack(Player& enemy, Position target) {
    ShotResult res = enemy.m_board.fireAt(target, enemy.m_ships);
    m_tracking.mark(target, res.type);
    return res;
}

bool Player::hasLost() const {
    for (const auto& s : m_ships) {
        if (!s.isSunk()) return false;
    }
    return true;
}
