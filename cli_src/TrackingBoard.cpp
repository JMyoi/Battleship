#include "TrackingBoard.h"

TrackingBoard::TrackingBoard() {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            m_track[r][c] = TrackState::Unknown;
        }
    }
}

TrackState TrackingBoard::at(Position p) const {
    if (p.r < 0 || p.r >= BOARD_SIZE || p.c < 0 || p.c >= BOARD_SIZE) {
        return TrackState::Unknown;
    }
    return m_track[p.r][p.c];
}

void TrackingBoard::mark(Position p, ShotResultType result) {
    if (p.r < 0 || p.r >= BOARD_SIZE || p.c < 0 || p.c >= BOARD_SIZE) return;

    // We only record meaningful outcomes for the attacker view
    if (result == ShotResultType::Miss) m_track[p.r][p.c] = TrackState::Miss;
    else if (result == ShotResultType::Hit || result == ShotResultType::Sunk) m_track[p.r][p.c] = TrackState::Hit;
    // Invalid / AlreadyTried: do not change tracking
}
