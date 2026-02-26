#pragma once
#include <vector>
#include "raylib.h"
using namespace std;

struct position{
    int row;
    int col;
};

class Ship{

    public:
        Ship(int size);
        void setPosition(vector<position> newPositions);
        //void setShipPosition();
        void drawShip(int i, Vector2 newPos);


    private:
        int size;
        vector<bool> hitMask; // going from right to left or top down
        vector<position> positions;
        //direction
        Rectangle shipRect; // for drawing the ship, will be set to the appropriate position and size when the ship is placed


};