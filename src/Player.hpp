#pragma once

#include "Board.hpp"
#include "Ship.hpp"
#include <vector>
//#include <string>

using namespace std;

class Player{
    public:
        Player(int shipCount);
        bool drawSetupBoard(); // returns true when all ships are placed and ready button is clicked, else false.
        void drawBoard(); // after setup phase board will display during players turn, should display ships, hits, misses, sunken ships, etc.
        bool drawTrackingBoard(ShotResult& res); //called during the opponents turn to display everything except where the ships are, handles fire and returns true if hit or miss so game can change state
        bool checkGameOver(); // returns true if all ships are sunk
        void placeShipsRandomly(); // AI setup: randomly places all ships on the board with legal placement
        void startReceivingFire(int row, int col, ShotResult& result); // AI fires at this player — fires board, registers hit on ship, starts animation
        bool drawBoardAITurn(); // draws own board during AI turn with animations; returns true when animation finishes
        
    private:
        Board playerBoard;
        vector<Ship> ships;
        // handles shot animation delay before transitioning to next game state
        bool shotPendingTransition; // stays true until animation is finished
        ShotResult pendingShotResult;

        void drawShipsonBoard();//draw the ships on the board, used by the drawSetupBoard and drawBoard funciton


};