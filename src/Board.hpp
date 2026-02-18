#pragma once
#include "raylib.h"
#include <vector>
using namespace std;

enum class TileState {Empty,Ship,Hit,Miss};

class Tile{
    public:
        Rectangle rect; // has x, y, width, height
        TileState state;
        Tile();
        void Draw();
};

class Board{
    public:
        Board();
        void Draw(Vector2 start, Vector2 end); 
    private:
        vector<vector<Tile>> grid; // 2D grid of tiles

};