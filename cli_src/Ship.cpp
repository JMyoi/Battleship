#include "Ship.h"

/* Construct a ship with a given size (length). */
Ship::Ship(int size)
    : m_size(size)
{
}

/* Return ship size. */
int Ship::size() const {
    return m_size;
}

/* Count how many segments have been hit. */
int Ship::hitCount() const {
    int count = 0;
    for (bool h : m_hitMask) {
        if (h) count++;
    }
    return count;
}

/* A ship is sunk when all its segments are hit. */
bool Ship::isSunk() const {
    return hitCount() >= m_size;
}

/* Get occupied cells. */
const std::vector<Position>& Ship::cells() const {
    return m_cells;
}

/* Set occupied cells and reset hit tracking. */
void Ship::setCells(const std::vector<Position>& positions) {
    m_cells = positions;
    m_hitMask.assign(positions.size(), false);
}

/* Register a hit if p is part of this ship and not previously hit. */
bool Ship::registerHit(Position p) {
    for (std::size_t i = 0; i < m_cells.size(); ++i) {
        if (m_cells[i].r == p.r && m_cells[i].c == p.c) {
            if (!m_hitMask[i]) {
                m_hitMask[i] = true;
                return true;
            }
            return false; // already hit this segment
        }
    }
    return false; // not part of this ship
}
