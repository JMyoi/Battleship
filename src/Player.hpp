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
        bool checkGameOver(); // returns true if all ships are hit, can be implementd by checking vector of ships, if all ship are sunk

        
    private:
        Board playerBoard;
        vector<Ship> ships;
        //string name;

        void drawShipsonBoard(); 

};