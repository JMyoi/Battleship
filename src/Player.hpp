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
        void drawTrackingBoard(); //called during the opponents turn to display everything except where the ships are.
        
    private:
        Board playerBoard;
        vector<Ship> ships;
        //string name;

        void drawShipsonBoard(); 

};