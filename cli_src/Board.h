#pragma once
#include <vector>
#include "Types.h"
#include "Ship.h"

/**
 * @class Board
 * @brief Represents a 10x10 game board for one player.
 *
 * The Board is responsible for:
 * - Validating and placing ships
 * - Tracking cell states (empty, ship, hit, miss)
 * - Processing attacks
 * - Coordinating with Ship objects to detect sunk ships
 *
 * It does NOT manage turns or overall game flow.
 */
class Board {
public:

    /// Construct an empty 10x10 board.
    Board();

    /**
     * @brief Checks if a ship can be placed at a given position and direction.
     * @param shipSize Length of the ship.
     * @param start Starting position.
     * @param dir Direction (horizontal or vertical).
     * @return true if placement is valid.
     */
    bool canPlaceShip(int shipSize, Position start, Direction dir) const;

    /**
     * @brief Places a ship on the board.
     * @param shipIndex Index of ship in player's ship list.
     * @param shipSize Length of the ship.
     * @param start Starting position.
     * @param dir Direction.
     * @param ships Reference to player's ships.
     * @return true if placement succeeds.
     */
    bool placeShip(int shipIndex,
                   int shipSize,
                   Position start,
                   Direction dir,
                   std::vector<Ship>& ships);

    /**
     * @brief Processes an attack at a given position.
     * @param p Target position.
     * @param ships Reference to player's ships.
     * @return Result of the shot (hit, miss, sunk, etc.).
     */
    ShotResult fireAt(Position p, std::vector<Ship>& ships);

    /// Returns the state of a specific cell.
    CellState cellAt(Position p) const;

private:

    /// Returns true if position is inside board boundaries.
    bool inBounds(Position p) const;

    CellState m_grid[BOARD_SIZE][BOARD_SIZE];
    ///< Stores cell state (Empty, Ship, Hit, Miss)

    int m_shipAt[BOARD_SIZE][BOARD_SIZE];
    ///< Stores which ship occupies each cell (-1 if none)
};
