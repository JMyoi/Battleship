#include "Board.h"

// Construct an empty board
Board::Board() {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            m_grid[r][c] = CellState::Empty;
            m_shipAt[r][c] = -1;
        }
    }
}

// Check bounds
bool Board::inBounds(Position p) const {
    return (p.r >= 0 && p.r < BOARD_SIZE && p.c >= 0 && p.c < BOARD_SIZE);
}

// Check if ship placement is valid (in bounds + no overlap)
bool Board::canPlaceShip(int shipSize, Position start, Direction dir) const {
    if (shipSize <= 0) return false;
    if (!inBounds(start)) return false;

    for (int i = 0; i < shipSize; ++i) {
        Position p = start;
        if (dir == Direction::Horizontal) {
            p.c += i;
        } else {
            p.r += i;
        }

        if (!inBounds(p)) return false;                 // out of board
        if (m_grid[p.r][p.c] != CellState::Empty) return false; // overlap
    }
    return true;
}

// Place ship: write Ship cells + update grid and ship mapping
bool Board::placeShip(int shipIndex,
                      int shipSize,
                      Position start,
                      Direction dir,
                      std::vector<Ship>& ships)
{
    // basic validation
    if (shipIndex < 0 || shipIndex >= (int)ships.size()) return false;
    if (!canPlaceShip(shipSize, start, dir)) return false;

    std::vector<Position> cells;
    cells.reserve(shipSize);

    for (int i = 0; i < shipSize; ++i) {
        Position p = start;
        if (dir == Direction::Horizontal) {
            p.c += i;
        } else {
            p.r += i;
        }

        cells.push_back(p);
        m_grid[p.r][p.c] = CellState::Ship;
        m_shipAt[p.r][p.c] = shipIndex;
    }

    // Sync cells into the Ship object
    ships[shipIndex].setCells(cells);
    return true;
}

// Fire at a position and return shot result
ShotResult Board::fireAt(Position p, std::vector<Ship>& ships) {
    ShotResult result;
    result.type = ShotResultType::Invalid;
    result.sunkShipIndex = -1;

    if (!inBounds(p)) {
        result.type = ShotResultType::Invalid;
        return result;
    }

    // Already shot here?
    if (m_grid[p.r][p.c] == CellState::Hit || m_grid[p.r][p.c] == CellState::Miss) {
        result.type = ShotResultType::AlreadyTried;
        return result;
    }

    // Miss
    if (m_grid[p.r][p.c] == CellState::Empty) {
        m_grid[p.r][p.c] = CellState::Miss;
        result.type = ShotResultType::Miss;
        return result;
    }

    // Hit (must be a ship cell)
    if (m_grid[p.r][p.c] == CellState::Ship) {
        m_grid[p.r][p.c] = CellState::Hit;
        result.type = ShotResultType::Hit;

        int shipIndex = m_shipAt[p.r][p.c];
        if (shipIndex >= 0 && shipIndex < (int)ships.size()) {
            ships[shipIndex].registerHit(p);

            if (ships[shipIndex].isSunk()) {
                result.type = ShotResultType::Sunk;
                result.sunkShipIndex = shipIndex;
            }
        }
        return result;
    }

    // Fallback (should not reach)
    result.type = ShotResultType::Invalid;
    return result;
}

// Read cell state
CellState Board::cellAt(Position p) const {
    if (!inBounds(p)) return CellState::Empty; // safe fallback
    return m_grid[p.r][p.c];
}
