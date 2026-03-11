#include <iostream>
#include <limits>
#include <cctype>
#include "Game.h"
#include <ctime>

const bool DEBUG_SHOW_AI_SHIPS = false;


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
        char colChar;
        int row;

        std::cout << prompt << " (e.g. A 1 to J 10): ";

        if (!(std::cin >> colChar >> row)) {
            ClearInput();
            continue;
        }

        colChar = static_cast<char>(std::toupper(static_cast<unsigned char>(colChar)));
        int c = colChar - 'A';
        int r = row - 1;

        if (c >= 0 && c < BOARD_SIZE && r >= 0 && r < BOARD_SIZE) {
            return Position{r, c};
        }

        std::cout << "Invalid input. Use A-J and 1-10.\n";
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

static void PrintBoard(const Board& board, bool showShips) {
    std::cout << "   A B C D E F G H I J\n";
    for (int r = 0; r < BOARD_SIZE; ++r) {
        if (r + 1 < 10) std::cout << " ";
        std::cout << (r + 1) << " ";

        for (int c = 0; c < BOARD_SIZE; ++c) {
            Position p{r, c};
            CellState cell = board.cellAt(p);

            char ch = '.';
            if (cell == CellState::Empty) ch = '.';
            else if (cell == CellState::Ship) ch = (showShips ? 'S' : '.');
            else if (cell == CellState::Hit) ch = 'X';
            else if (cell == CellState::Miss) ch = 'O';

            std::cout << ch << " ";
        }
        std::cout << "\n";
    }
}

static int ReadShipCount() {
    while (true) {
        int shipCount;
        std::cout << "Enter number of ships (1-5): ";
        if (!(std::cin >> shipCount)) {
            ClearInput();
            continue;
        }
        if (shipCount >= 1 && shipCount <= 5) {
            return shipCount;
        }
        std::cout << "Please enter a number from 1 to 5.\n";
    }
}

static int ReadMode() {
    while (true) {
        int mode;
        std::cout << "\nChoose game mode:\n";
        std::cout << "1. Player vs Player\n";
        std::cout << "2. Player vs AI\n";
        std::cout << "Enter choice: ";
        if (!(std::cin >> mode)) {
            ClearInput();
            continue;
        }
        if (mode == 1 || mode == 2) {
            return mode;
        }
        std::cout << "Invalid choice. Enter 1 or 2.\n";
    }
}

static AIDifficulty ReadAIDifficulty() {
    while (true) {
        int diff;
        std::cout << "\nChoose AI difficulty:\n";
        std::cout << "1. Easy\n";
        std::cout << "2. Medium\n";
        std::cout << "3. Hard\n";
        std::cout << "Enter choice: ";
        if (!(std::cin >> diff)) {
            ClearInput();
            continue;
        }

        if (diff == 1) return AIDifficulty::Easy;
        if (diff == 2) return AIDifficulty::Medium;
        if (diff == 3) return AIDifficulty::Hard;

        std::cout << "Invalid choice. Enter 1, 2, or 3.\n";
    }
}

int main() {
    srand(time(nullptr));
    std::cout << "=== Battleship (CLI Version) ===\n";

    int shipCount = ReadShipCount();
    int mode = ReadMode();

    Game game(
        shipCount,
        PlayerType::Human,
        (mode == 1 ? PlayerType::Human : PlayerType::AI),
        AIDifficulty::None,
        (mode == 1 ? AIDifficulty::None : ReadAIDifficulty())
    );

    int placedP1 = 0;
    int placedP2 = 0;

    // Setup Phase
    while (game.state() == GameState::SetupP1 ||
           game.state() == GameState::SetupP2) {

        int current = game.currentPlayerIndex();
        Player& curPlayer = game.player(current);

        if (curPlayer.isAI()) {
            std::cout << "\n--- AI is placing ships automatically ---\n";
            if (game.autoPlaceShipsForCurrentPlayer()) {
                std::cout << "AI ships placed successfully.\n";

                if (DEBUG_SHOW_AI_SHIPS) {
                    std::cout << "\n[DEBUG] AI Board After Placement:\n";
                    PrintBoard(curPlayer.board(), true);
                }
            } else {
                std::cout << "AI ship placement failed.\n";
                return 1;
            }
            continue;
        }

        int shipIndex = (current == 0) ? placedP1 : placedP2;
        int shipSize = shipIndex + 1;

        std::cout << "\n--- Player " << (current + 1)
                  << " place ship of size " << shipSize << " ---\n";

        PrintBoard(curPlayer.board(), true);

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
        Player& curPlayer = game.player(current);

        std::cout << "\n=== Player " << (current + 1) << " Turn ===\n";

        if (mode == 2 && DEBUG_SHOW_AI_SHIPS) {
            std::cout << "\n[DEBUG] AI Board:\n";
            PrintBoard(game.player(1).board(), true);
        }

        if (curPlayer.isAI()) {
            std::cout << "AI is thinking...\n";
            ShotResult result = game.fireAITurn();
            PrintShot(result);
        } else {
            Position target = ReadPosition("Fire at");
            ShotResult result = game.fire(target);
            PrintShot(result);
        }
    }

    std::cout << "\n*** Game Over! Winner: Player "
              << (game.winnerIndex() + 1)
              << " ***\n";

    return 0;
}