#include "Player.h"
#include <cstdlib>

Player::Player(int shipCount, PlayerType type, AIDifficulty difficulty)
    : m_type(type), m_difficulty(difficulty)
{
    if (shipCount < 1) shipCount = 1;
    if (shipCount > 5) shipCount = 5;

    m_ships.reserve(shipCount);
    for (int i = 0; i < shipCount; ++i) {
        m_ships.emplace_back(i + 1);
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

bool Player::isAI() const {
    return m_type == PlayerType::AI;
}

PlayerType Player::type() const {
    return m_type;
}

AIDifficulty Player::difficulty() const {
    return m_difficulty;
}

bool Player::isValidPosition(Position p) const {
    return p.r >= 0 && p.r < BOARD_SIZE && p.c >= 0 && p.c < BOARD_SIZE;
}

bool Player::isUntriedPosition(Position p) const {
    return m_tracking.at(p) == TrackState::Unknown;
}

void Player::addMediumNeighbors(Position p) {
    Position neighbors[4] = {
        {p.r - 1, p.c},
        {p.r + 1, p.c},
        {p.r, p.c - 1},
        {p.r, p.c + 1}
    };

    for (const auto& n : neighbors) {
        if (isValidPosition(n) && isUntriedPosition(n)) {
            bool exists = false;
            for (const auto& pos : m_mediumTargets) {
                if (pos.r == n.r && pos.c == n.c) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                m_mediumTargets.push_back(n);
            }
        }
    }
}

Position Player::chooseAITarget(const Player& enemy) {
    if (!isAI()) return {-1, -1};

    if (m_difficulty == AIDifficulty::Easy) {
        while (true) {
            Position p{ rand() % BOARD_SIZE, rand() % BOARD_SIZE };
            if (isUntriedPosition(p)) return p;
        }
    }

    if (m_difficulty == AIDifficulty::Medium) {
        while (!m_mediumTargets.empty()) {
            Position p = m_mediumTargets.back();
            m_mediumTargets.pop_back();
            if (isUntriedPosition(p)) return p;
        }

        while (true) {
            Position p{ rand() % BOARD_SIZE, rand() % BOARD_SIZE };
            if (isUntriedPosition(p)) return p;
        }
    }

    if (m_difficulty == AIDifficulty::Hard) {
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                Position p{r, c};
                if (!isUntriedPosition(p)) continue;

                if (enemy.board().hasShipAt(p)) {
                    return p;
                }
            }
        }
    }

    while (true) {
        Position p{ rand() % BOARD_SIZE, rand() % BOARD_SIZE };
        if (isUntriedPosition(p)) return p;
    }
}

void Player::updateAIStateAfterShot(Position target, ShotResult result) {
    if (!isAI()) return;
    if (m_difficulty != AIDifficulty::Medium) return;

    if (result.type == ShotResultType::Hit) {
        addMediumNeighbors(target);
    }
    else if (result.type == ShotResultType::Sunk) {
        m_mediumTargets.clear();
    }
}
bool Player::placeAllShipsRandomly() {
    for (int i = 0; i < (int)m_ships.size(); ++i) {
        bool placed = false;
        int shipSize = i + 1;

        while (!placed) {
            Position start{ rand() % BOARD_SIZE, rand() % BOARD_SIZE };
            Direction dir = (rand() % 2 == 0) ? Direction::Horizontal : Direction::Vertical;

            placed = m_board.placeShip(i, shipSize, start, dir, m_ships);
        }
    }
    return true;
}