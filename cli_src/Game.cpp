#include "Game.h"

Game::Game(int shipCount)
    : m_shipCount(shipCount),
      m_state(GameState::SetupP1),
      m_nextShipToPlace(0),
      m_winner(-1),
      m_p1(shipCount),
      m_p2(shipCount)
{
    if (m_shipCount < 1) m_shipCount = 1;
    if (m_shipCount > 5) m_shipCount = 5;
}

GameState Game::state() const { return m_state; }
int Game::currentPlayerIndex() const {
    if (m_state == GameState::SetupP1 || m_state == GameState::TurnP1) return 0;
    if (m_state == GameState::SetupP2 || m_state == GameState::TurnP2) return 1;
    return -1;
}
int Game::winnerIndex() const { return m_winner; }

Player& Game::player(int idx) { return (idx == 0) ? m_p1 : m_p2; }
const Player& Game::player(int idx) const { return (idx == 0) ? m_p1 : m_p2; }

bool Game::placeNextShip(Position start, Direction dir) {
    if (m_state != GameState::SetupP1 && m_state != GameState::SetupP2) return false;

    Player& cur = (m_state == GameState::SetupP1) ? m_p1 : m_p2;
    int shipIndex = m_nextShipToPlace;
    int shipSize  = shipSizeForIndex(shipIndex);

    bool ok = cur.board().placeShip(shipIndex, shipSize, start, dir, cur.ships());
    if (!ok) return false;

    m_nextShipToPlace++;
    advanceSetup();
    return true;
}

void Game::advanceSetup() {
    if (m_nextShipToPlace < m_shipCount) return;

    // finished placing for current setup player
    m_nextShipToPlace = 0;
    if (m_state == GameState::SetupP1) m_state = GameState::SetupP2;
    else m_state = GameState::TurnP1; // after P2 setup, start P1 turn
}

ShotResult Game::fire(Position target) {
    ShotResult invalid{ShotResultType::Invalid, -1};

    if (m_state != GameState::TurnP1 && m_state != GameState::TurnP2) return invalid;

    Player& attacker = (m_state == GameState::TurnP1) ? m_p1 : m_p2;
    Player& defender = (m_state == GameState::TurnP1) ? m_p2 : m_p1;

    ShotResult res = attacker.attack(defender, target);

    checkGameOver();
    if (!isOver()) {
        // Only switch turn when the shot was a valid attempt (Hit/Miss/Sunk)
        if (res.type == ShotResultType::Hit || res.type == ShotResultType::Miss || res.type == ShotResultType::Sunk) {
            advanceTurn();
        }
    }
    return res;
}

void Game::advanceTurn() {
    if (m_state == GameState::TurnP1) m_state = GameState::TurnP2;
    else if (m_state == GameState::TurnP2) m_state = GameState::TurnP1;
}

void Game::checkGameOver() {
    if (m_p1.hasLost()) { m_state = GameState::GameOver; m_winner = 1; }
    else if (m_p2.hasLost()) { m_state = GameState::GameOver; m_winner = 0; }
}

bool Game::isOver() const { return m_state == GameState::GameOver; }
