#pragma once

const int BOARD_SIZE = 10;
/// The project specification requires a 10x10 board.

struct Position {
    int r; ///< Row index
    int c; ///< Column index
};
/**
 * @brief Represents ship placement direction.
 */
enum class Direction {
    Horizontal,
    Vertical
};
/**
 * @brief Represents the state of a board cell.
 */
enum class CellState {
    Empty,
    Ship,
    Hit,
    Miss
};

/**
 * @brief Represents the result of a shot.
 */
enum class ShotResultType {
    Invalid,
    AlreadyTried,
    Miss,
    Hit,
    Sunk
};

struct ShotResult {
    ShotResultType type;
    int sunkShipIndex;  // -1 if not sunk
};
