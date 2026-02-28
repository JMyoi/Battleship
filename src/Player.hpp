#pragma once

#include "Board.hpp"
#include "Ship.hpp"
#include <vector>
//#include <string>

using namespace std;

class Player{
    public:
        Player(int shipCount);
        void drawSetupBoard();
        
    private:
        Board playerBoard;
        vector<Ship> ships;
        //string name;

};