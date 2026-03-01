#pragma once
#include "Player.hpp"

using namespace std;


enum class GameState { Menu, 
    SetupP1, SetupP2,
    P1Transition, P2Transition,
    TurnP1, TurnP2, 
    GameOver };


class Game{
    public:
        Game();
        GameState getGameState();
        void drawMenu();
        void drawP1Setup();
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
        int NoOfShips;

        int getSelectedOption(vector<Rectangle>& options);
        void updateGameState(GameState newState); // not used and not nessecary

        

};