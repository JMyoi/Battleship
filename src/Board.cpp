#include "Board.hpp"
#include "raylib.h"
#include <vector>
#include <iostream>
using namespace std;

//Tile Implementation

Tile::Tile(){
    rect = {0,0,0,0}; // width and height should be 50. 
    state = TileState::Empty;
}

void Tile::Draw(){
    DrawRectangleLinesEx(rect, 1, BLACK );
}


//Board Implementation


//constructor
Board::Board(){
    grid.resize(10);
    for(int i = 0; i < 10; i++){
        grid.at(i).resize(10);
    }

}


//draws the players board, 
void Board::Draw(Vector2 start){
    // float BoardWidth = 500; // 500
    // float BoardHeight = 500; // 500
    float TileWidth = 50; // board width / 10 = 500 / 10 = 50
    float TileHeight = 50; // 50
    float TileX = start.x;  
    float TileY = start.y; 

    // assign all tiles appropriate width, height,  and position 
    for(int row = 0; row<10; row++){
        TileX = start.x;  // Reset X at the start of each row
        for(int col = 0; col<10; col++){
            grid.at(row).at(col).rect.width = TileWidth;
            grid.at(row).at(col).rect.height = TileHeight;
            grid.at(row).at(col).rect.x = TileX;
            grid.at(row).at(col).rect.y = TileY;
            grid.at(row).at(col).Draw();
            TileX += TileWidth;  // Move to next column
           
        }
        TileY += TileHeight;  // Move to next row
    }

    // for(int row = 0; row<10; row++){
    //     for(int col = 0; col<10; col++){
    //         grid.at(row).at(col).Draw();
    //     }
    // }

}