#pragma once
#include "Player.hpp"

using namespace std;

enum class GameState { Menu, 
    SetupP1, SetupTransitionP2, SetupP2, P1Transition, 
    TurnP1, P2Transition, TurnP2, 
    GameOver };


class Game{
    public:
        Game();
        void updateGameState();
        void DrawMenu();
        void DrawSetup();
        void DrawGame();
        void DrawGameOver();

    private:
        Player player1;
        Player player2;
        GameState state;

};