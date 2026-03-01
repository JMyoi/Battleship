#include "Board.hpp"
#include "raylib.h"
#include "Ship.hpp"
#include <vector>
#include <iostream>
using namespace std;

//Tile Implementation

Tile::Tile(){
    rect = {0,0,0,0}; 
    state = TileState::Empty;
}

void Tile::Draw(){
    DrawRectangleLinesEx(rect, 1, BLACK );
}
bool Tile::isClicked(){
    if(CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return true;
    return false;
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

}

//returns the positions where it was placed.
bool Board::HandlePlaceShip(int shipSize, vector<position>& newPositions, Direction direction){
    for(int row = 0; row<10; row++){
        for(int col = 0; col<10; col++){

            if(grid.at(row).at(col).isClicked()){
                //is this tile empty and there is no ship on it, then maybe we can place it.
                if(grid.at(row).at(col).state == TileState::Empty){
                    bool canPlace = true;
                    int currRow = row, currCol = col;
                    // detect if the other parts of the ship are out of bound, !empty, or has a ship already.
                    //for the respective direction, vertical or horizontal
                    if(direction == Direction::Vertical){
                        for(int i = 0; i<shipSize-1; i++){
                            // to access the vertical directions index, we do x+1, for shipSize-1 times.
                            currRow++; // for vertical placement col does not change, only X, row.
                            bool outOfBound = currRow>=10;
                            // check for not out of bound and tile is empty.
                            if((!outOfBound) && (grid.at(currRow).at(col).state == TileState::Empty)){
                                // all conditions met, canPlace remains true
                                }
                            else{
                                cout<<"ship cannot be placed, either out of bound or ship already there.\n";
                                return canPlace = false; 
                            }
                        }
                        if(canPlace){
                            // place ship on board, update tile states to ship.
                            currRow = row; // reset X
                            for(int i = 0; i<shipSize; i++){
                                grid.at(currRow).at(col).state = TileState::Ship;
                                newPositions.at(i).row = currRow;
                                newPositions.at(i).col = col;
                                newPositions.at(i).hit = false;
                                cout<<"Ship is placed at: Row:"<<currRow<<", Col: "<<col<<endl;
                                currRow++;
                            }
                            return true;
                        }
                        // detect if ship trying to be placed is colliding with other ships that are already placed.
                        //edgecase? shipsize is one.
                    }
                    else if(direction == Direction::Horizontal){
                         for(int i = 0; i<shipSize-1; i++){
                            // to access the vertical directions index, we do curCol+1, for shipSize-1 times.
                            currCol++; 
                            bool outOfBound = currCol>=10;
                            // check for not out of bound and tile is empty.
                            if((!outOfBound) && (grid.at(row).at(currCol).state == TileState::Empty)){
                                // all conditions met, canPlace remains true
                                }
                            else{
                                cout<<"ship cannot be placed, either out of bound or ship already there.\n";
                                return canPlace = false; 
                            }
                        }
                        if(canPlace){
                            // place ship on board, update tile states to ship.
                            currCol = col; // reset X
                            for(int i = 0; i<shipSize; i++){
                                grid.at(row).at(currCol).state = TileState::Ship;
                                newPositions.at(i).row = row;
                                newPositions.at(i).col = currCol;
                                newPositions.at(i).hit = false;
                                cout<<"Ship is placed at: Row: "<<row<<" Col: "<<currCol<<endl;
                                currCol++;
                            }
                            return true;
                        }
                        
                    }

                }else{
                    cout<<"ship cannot be placed ship already there.\n";
                    return false;
                }
            }
        }
    }
    return false;
}