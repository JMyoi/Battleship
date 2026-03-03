#pragma once
#include "Player.hpp"
#include "Board.hpp"


using namespace std;


enum class GameState { Menu, 
    SetupP1, P2SetupTransition, SetupP2,
    P1Transition, P2Transition,
    TurnP1, TurnP2, 
    GameOver };

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
        int NoOfShips;
        ShotResult CurrResult;// keeps track and displays the players result after each fire, it's the first turn there will be no firing history and state will be AlreadyFired to indicate that, assigned by constructor
        int winner = 0; 
        // Stores the final winner of the match.
        // 0 = no winner yet
        // 1 = Player 1 wins
        // 2 = Player 2 wins
        // This value is set when the game transitions to GameOver.


        int getSelectedOption(vector<Rectangle>& options);
        void updateGameState(GameState newState); // not used and not nessecary

        

};