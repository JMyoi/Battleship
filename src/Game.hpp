#pragma once
#include "Player.hpp"
#include "Board.hpp"


using namespace std;


enum class GameState { Menu,
    SetupP1, P2SetupTransition, SetupP2,
    P1Transition, P2Transition,
    TurnP1, TurnP2,
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

        //helper private function for menu — writes clicked index into `selected`
        void getSelectedOption(vector<Rectangle>& options, int& selected);

        

};