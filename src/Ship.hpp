#pragma once
#include <vector>
#include "raylib.h"
using namespace std;

struct position{
    int row;
    int col;
    bool hit;// for if that ship unit is hit or miss, 
};
enum class Direction {
    Horizontal, Vertical
};


class Ship{

    public:
        Ship(int size);
        void setPosition(vector<position> newPositions);
        void drawShip(Vector2 newPos);
        bool shipClicked();


    private:
        int size;
        //vector<bool> hitMask; // going from right to left or top down
        vector<position> positions;
        //direction
        Rectangle shipRect; // for drawing the ship, will be set to the appropriate position and size when the ship is placed


};