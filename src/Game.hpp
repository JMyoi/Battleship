#pragma once
#include "Player.hpp"
#include "Board.hpp"
#include <set>
#include <utility>

using namespace std;


enum class GameState { Menu,
    SetupP1, P2SetupTransition, SetupP2,
    P1Transition, P2Transition,
    TurnP1, TurnP2,
    AITurn,
    GameOver };

enum class GameMode { LocalPvP, AIEasy, AIMedium, AIHard };

class Game{
    public:
        Game();
        GameState getGameState();
        void drawMenu();
        void drawP1Setup();
        void drawP2SetupTransition();
        void drawP2Setup();
        void drawP1Transition();
        void drawP2Transition();
        void drawP1Turn();
        void drawP2Turn();
        void drawAITurn();
        void drawGameOver();
    
    private:
        Player player1;
        Player player2;
        GameState state;
        GameMode gameMode;
        int NoOfShips;
        // keeps track and displays the players result after each fire,(Did they hit or miss?)
        //if it's the first turn there will be no firing history and state will
        //be AlreadyFired to indicate that, assigned by constructor
        ShotResult CurrResult;

        // AI state — shared
        bool aiShotPending;             // true while waiting for AI shot animation to finish
        pair<int,int> aiLastShot;       // (row, col) of the AI's most recent shot, for display
        set<pair<int,int>> aiFiredAt;   // every cell the AI has fired at

        // Medium AI targeting state (Hunt/Target algorithm)
        bool aiHunting;              // true = committed to sinking a hit ship
        pair<int,int> aiHitAnchor;   // position of the first hit on the targeted ship
        pair<int,int> aiLockedDir;   // direction being extended; {0,0} = not yet locked
        pair<int,int> aiExtendFrom;  // frontier: the last hit position to extend from

        pair<int,int> computeAIShot();

        //helper private function for menu — writes clicked index into `selected`
        void getSelectedOption(vector<Rectangle>& options, int& selected);

};