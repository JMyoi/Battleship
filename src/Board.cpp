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

    // crop the top right corner from poof.png
    Image img = LoadImage("src/assets/Poof.png");
    //width 2000, height 1400
    float h = static_cast<float>(img.height/3); 
    float w = static_cast<float>(img.width/3); 
    Rectangle crop = {0, 0, w, h};
    ImageCrop(&img, crop);
    crop = {300, 80, 170, 170};
    ImageCrop(&img, crop);
    ImageResize(&img,50,50);
    impact = LoadTextureFromImage(img);     

    //load the miss splash
    img = LoadImage("src/assets/drip.png");
    crop = {570, 400,340,270};
    ImageCrop(&img, crop);
    ImageResize(&img,50,50);
    missSplash = LoadTextureFromImage(img);  

}


void Tile::Draw(){
    DrawRectangleLinesEx(rect, 1, BLACK );
}

void Tile::drawHitsAndMiss(){
    int centerX = rect.x + 25; // height and width is default 50, 50/2 = 25.
    int centerY = rect.y + 25;
    // based on the state, display different color, red for hit, blue for miss, nothing for miss.
    switch (state)
    {
    case TileState::Hit:{
       //DrawCircle(centerX, centerY, 10, RED);
       DrawTextureV(impact, {rect.x, rect.y}, WHITE);
        break;
    }
    case TileState::Miss:{
        DrawCircle(centerX, centerY, 10, BLUE);
        DrawTextureV(missSplash, {rect.x, rect.y}, WHITE);
        //render the drip
         break;
    }
    case TileState::Empty:
    case TileState::Ship:
        break;
    }

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

    //render background ocean
    Image img = LoadImage("src/assets/Ocean.png");
    ImageResize(&img, 500, 500);
    Ocean = LoadTextureFromImage(img);
}


//draws the players board, 
void Board::Draw(Vector2 start){
    //background ocean
    DrawTextureV(Ocean,start,WHITE);
    float TileWidth = 50; // board width / 10 = 500 / 10 = 50
    float TileHeight = 50; // 50
    float TileX = start.x;  
    float TileY = start.y; 
    int fontSize = 20;
    float offsetTop = 30;
    float offsetLeft = 30;

    // Column labels: A-J (bottom)
    for (int col = 0; col < 10; col++) {
        char letter = 'A' + col;
        DrawText(TextFormat("%c", letter),
                (int)(start.x + col * TileWidth + 18),
                (int)(start.y + 10 * TileHeight + 10), // below the grid
                fontSize,
                BLACK);
    }   
    // Row labels: 1-10
    for (int row = 0; row < 10; row++) {
        DrawText(TextFormat("%d", row + 1),
                (int)(start.x - offsetLeft),
                (int)(start.y + row * TileHeight + TileHeight * 0.30f),
                fontSize,
                BLACK);
    }


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
void Board::DrawHitsAndMiss(Vector2 start){
    // assign all tiles appropriate width, height,  and position 
        for(int row = 0; row<10; row++){
            for(int col = 0; col<10; col++){
                grid.at(row).at(col).drawHitsAndMiss();
            }
        }

}

/* Trackingboard funciton calls this funciton,
should return true only if a tile is clicked
false otherwise
should check for clicks on the board and update result parameter accordingly
    if tileSate == Empty, return Miss
    if TileState == Ship, return Hit
    if TileState == Hit, return AlreadyFired
    if TileState == Miss, return AlreadyFired
should return the tile it has fired at if hit so players ship vector can update it's state.
*/ 
bool Board::HandleFire(ShotResult& result, position& at){
    for(int row = 0; row<10; row++){
        for(int col = 0; col<10; col++){
            if(grid.at(row).at(col).isClicked()){
                TileState thisTileState = grid.at(row).at(col).state;
                switch (thisTileState)
                {
                case TileState::Empty:
                    grid.at(row).at(col).state = TileState::Miss;// update tile state on board to hit.
                    result = ShotResult::Miss;
                    at.col = col;
                    at.row = row;
                    at.hit = false;
                    //should play the miss splash animation on that tile
                    return true;
                    break;
                case TileState::Ship:
                    grid.at(row).at(col).state = TileState::Hit;// update tile state on board to hit.
                    result = ShotResult::Hit;
                    at.col = col;
                    at.row = row;
                    at.hit = true;
                    // should play the explosion animation on that tile.
                    return true;
                    break;
                case TileState::Hit:
                case TileState::Miss:
                    result = ShotResult::AlreadyFired;
                    at.col = col;
                    at.row = row;
                    at.hit = true;
                    return true;
                    break;
                }

            }
        }
    }
    return false;
}

//returns the positions where it was placed.
bool Board::HandlePlaceShip(int shipSize, vector<position>& newPositions, Direction direction, string& ErrorMessage){
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
                                ErrorMessage = "Cannot place, either out of bound or ship already there";
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
                    }
                    else if(direction == Direction::Horizontal){
                         for(int i = 0; i<shipSize-1; i++){
                            // to access the horizontal directions index, we do curCol+1, for shipSize-1 times.
                            currCol++; 
                            bool outOfBound = currCol>=10;
                            // check for not out of bound and tile is empty.
                            if((!outOfBound) && (grid.at(row).at(currCol).state == TileState::Empty)){
                                // all conditions met, canPlace remains true
                                }
                            else{
                                cout<<"ship cannot be placed, either out of bound or ship already there.\n";
                                ErrorMessage = "Cannot place, either out of bound or ship already there";

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
                    ErrorMessage = "Cannot place, ship already there.";
                    return false;
                }
            }
        }
    }
    return false;
}