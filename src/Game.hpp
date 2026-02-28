#pragma once
#include "Player.hpp"

using namespace std;

int getSelectedOption(vector<Rectangle>& options);

enum class GameState { Menu, 
    SetupP1, SetupTransitionP2, SetupP2, P1Transition, 
    TurnP1, P2Transition, TurnP2, 
    GameOver };


class Game{
    public:
        Game();
        void updateGameState(GameState newState);
        GameState getGameState();
        void drawMenu();
        void drawSetup();
        void drawGame();
        void drawGameOver();

    private:
        Player player1;
        Player player2;
        GameState state;
        int NoOfShips;
        

};