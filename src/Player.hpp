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
        
        
    private:
        Board playerBoard;
        vector<Ship> ships;
        //string name;

        void drawShipsonBoard(); 

};