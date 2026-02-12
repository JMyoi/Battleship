#pragma once
#include <vector>
#include "Types.h"
/*
A ship has:
    A fixed size (length)
    A set of board positions it occupies
    A record of which positions have been hit
    */
class Ship {
public:
/**
     * @brief Construct a ship with a given size.
     * @param size Length of the ship (1–5).
     */
    Ship(int size = 1);

    int size() const;
    /// Returns the size of the ship.
    int hitCount() const;
    /// Returns how many times this ship has been hit.
    bool isSunk() const;
    /// Returns the board cells occupied by this ship.

    const std::vector<Position>& cells() const;
    /// Returns the board cells occupied by this ship
    void setCells(const std::vector<Position>& positions);
    /// Assigns board positions to this ship.
    bool registerHit(Position p);
    /// return true if the ship was successfully hit.

private:
    int m_size;
    std::vector<Position> m_cells;
    ///< Positions occupied on the board
    std::vector<bool> m_hitMask;
    ///< Tracks which positions have been hit
};
