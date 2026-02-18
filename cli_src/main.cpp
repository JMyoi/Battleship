#include <iostream>
#include <limits>
#include "Game.h"

static void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static Direction ReadDirection() {
    while (true) {
        char ch;
        std::cout << "Direction (H/V): ";
        if (!(std::cin >> ch)) {
            ClearInput();
            continue;
        }
        if (ch == 'H' || ch == 'h') return Direction::Horizontal;
        if (ch == 'V' || ch == 'v') return Direction::Vertical;

        std::cout << "Invalid input. Enter H or V.\n";
    }
}

static Position ReadPosition(const char* prompt) {
    while (true) {
        int r, c;
        std::cout << prompt << " (row col 0-9 0-9): ";
        if (!(std::cin >> r >> c)) {
            ClearInput();
            continue;
        }
        return Position{r, c};
    }
}

static void PrintShot(const ShotResult& r) {
    switch (r.type) {
        case ShotResultType::Invalid:
            std::cout << "Invalid position.\n";
            break;
        case ShotResultType::AlreadyTried:
            std::cout << "Already tried.\n";
            break;
        case ShotResultType::Miss:
            std::cout << "Miss.\n";
            break;
        case ShotResultType::Hit:
            std::cout << "Hit!\n";
            break;
        case ShotResultType::Sunk:
            std::cout << "Sunk! Ship index: " << r.sunkShipIndex << "\n";
            break;
        default:
            break;
    }
}

int main() {
    std::cout << "=== Battleship (CLI Version) ===\n";

    int shipCount;
    std::cout << "Enter number of ships (1-5): ";
    if (!(std::cin >> shipCount)) return 0;

    Game game(shipCount);

    // Setup Phase
    while (game.state() == GameState::SetupP1 ||
           game.state() == GameState::SetupP2) {

        int current = game.currentPlayerIndex();
        static int placedP1 = 0;
        static int placedP2 = 0;

        int shipIndex = (current == 0) ? placedP1 : placedP2;
        int shipSize = shipIndex + 1;

        std::cout << "\n--- Player " << (current + 1)
                  << " place ship of size " << shipSize << " ---\n";

        Position start = ReadPosition("Start position");
        Direction dir = ReadDirection();

        if (game.placeNextShip(start, dir)) {
            std::cout << "Placed successfully.\n";
            if (current == 0) placedP1++;
            else placedP2++;
        } else {
            std::cout << "Invalid placement. Try again.\n";
        }
    }

    // Turn Phase
    while (!game.isOver()) {
        int current = game.currentPlayerIndex();
        std::cout << "\n=== Player " << (current + 1) << " Turn ===\n";

        Position target = ReadPosition("Fire at");
        ShotResult result = game.fire(target);
        PrintShot(result);
    }

    std::cout << "\n*** Game Over! Winner: Player "
              << (game.winnerIndex() + 1)
              << " ***\n";

    return 0;
}
