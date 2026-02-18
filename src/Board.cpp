#include "Board.hpp"
#include "raylib.h"
#include <vector>
using namespace std;

//Tile Implementation

Tile::Tile(){
    rect = {0,0,0,0};
    state = TileState::Empty;
}

void Tile::Draw(){
    DrawRectangleLinesEx(rect, 1, BLACK );
}



//Board Implementation

Board::Board(){
    grid.resize(10);
    for(int i = 0; i < 10; i++){
        grid[i].resize(10);
    }
}

void Board::Draw(Vector2 start, Vector2 end){
    float BoardWidth = end.x - start.x;
    float BoardHeight = end.y - start.y;
    float TileWidth = BoardWidth / 10;
    float TileHeight = BoardHeight / 10;
    float TileX = start.x;
    float TileY = start.y;

    // assign all tiles appropriate width, height and position
    for(int row = 0; row<10; row++){
        TileX = start.x;  // Reset X at the start of each row
        for(int col = 0; col<10; col++){
            grid.at(row).at(col).rect.width = TileWidth;
            grid.at(row).at(col).rect.height = TileHeight;
            grid.at(row).at(col).rect.x = TileX;
            grid.at(row).at(col).rect.y = TileY;
            TileX += TileWidth;  // Move to next column
           
        }
        TileY += TileHeight;  // Move to next row
    }
    for(int row = 0; row<10; row++){
        
        for(int col = 0; col<10; col++){
            grid.at(row).at(col).Draw();
           
        }

    }

}