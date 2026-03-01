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
        void setShip(const vector<position>& newPositions);// if a ship is placed on the board this is called to set the ships position and direction.
        void drawShip(Vector2 newPos);
        void drawShipHorizontal(Vector2 newPos);
        void draw(); // draw the ship that has a position set from placement. 
        bool shipClicked();
        bool isPlaced();


    private:
        int size;
        //vector<bool> hitMask; // going from right to left or top down
        vector<position> positions;
        Rectangle shipRect; // for drawing the ship, will be set to the appropriate position and size when the ship is placed
        Direction direction; 


};