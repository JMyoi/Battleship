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
        void drawShip(Vector2 newPos);// vertical by default
        void drawShipHorizontal(Vector2 newPos);
        void draw(); // draw the ship that has a position set from placement. 
        bool shipClicked();
        bool isPlaced();
        void shipHitAt(position pos);
        bool matchingAt(position pos); // returns true if there are any ship unit positions that match the given arguments position.
        bool isSunk();

    private:
        int size;
        vector<position> positions;
        Rectangle shipRect; // registering ship hits on click, will be set to the appropriate position and size when the ship is placed
        Direction direction; 
        //ship texture and image based on size
        Texture2D sprite;



};