#pragma once
#include "raylib.h"
#include "Ship.hpp"
#include <vector>
using namespace std;

enum class TileState {Empty,Ship,Hit,Miss};

class Tile{
    public:
        // data members
        Rectangle rect; // has x, y, width, height
        TileState state;
        // member funcitons
        Tile();
        void Draw();
        bool isClicked();
};

class Board{
    public:
        Board();
        void Draw(Vector2 start); 
        bool HandlePlaceShip(int shipSize, vector<position>& newPositions, Direction direction); // returns true if the ship is placed and false if not.
    private:
        vector<vector<Tile>> grid; // 2D grid of tiles

};