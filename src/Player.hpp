#pragma once

#include "Board.hpp"
#include "Ship.hpp"
#include <vector>
using namespace std;

class Player{
    public:
        Player(int shipCount);

    private:
        Board playerBoard;
        vector<Ship> ships;

};