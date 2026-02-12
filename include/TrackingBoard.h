#pragma once
#include "Types.h"

/**
 * @brief What the attacker knows about opponent cells.
 */
enum class TrackState { Unknown, Miss, Hit };

class TrackingBoard {
public:
    TrackingBoard();

    TrackState at(Position p) const;

    // Mark result after firing at opponent: Hit/Miss/AlreadyTried/Invalid/Sunk
    void mark(Position p, ShotResultType result);

private:
    TrackState m_track[BOARD_SIZE][BOARD_SIZE];
};
