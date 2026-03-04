#pragma once
#include "raylib.h"
#include "Ship.hpp"
#include <string>
#include <vector>
using namespace std;

enum class TileState {Empty,Ship,Hit,Miss};
enum class ShotResult{Hit, Miss, AlreadyFired};

class Tile{
    public:
        // data members
        Rectangle rect; // has x, y, width, height
        TileState state;
        Texture impact; // used to indicate hit
        Texture missSplash;//used to indicate miss
        // member funcitons
        Tile();
        void Draw();
        void drawHitsAndMiss();// called by Board::DrawHitAndMiss()
        bool isClicked();
};

class Board{
    public:
        Board();
        void Draw(Vector2 start); 
        void DrawHitsAndMiss(Vector2 start); // because the ship renders over the hits form Draw(), we need a separate funciton to call after the ships are rendered.
        bool HandlePlaceShip(int shipSize, vector<position>& newPositions, Direction direction,string& ErrorMessage); // returns true if the ship is placed and false if not.
        bool HandleFire(ShotResult& result, position& at); // checks for a click on board and tries to verify hit or miss.
    private:
        vector<vector<Tile>> grid; // 2D grid of tiles
        Texture2D Ocean;

};