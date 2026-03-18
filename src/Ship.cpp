#include "Ship.hpp"
#include <vector>
#include <iostream>
#include "raylib.h"
#include <string>
using namespace std;


Ship::Ship(int size){
    this->size = size;
    direction = Direction::Vertical; // by default, may change during setup phase
    int height = 50 * size;
    shipRect = {0, 0, 50, float(height)};
    positions.resize(size);
    for(int i = 0; i< positions.size(); i++){
        positions.at(i).hit = false;
        positions.at(i).row = -1;
        positions.at(i).col = -1;
    }
    //load the ace sound
    Ace = LoadSound("src/assets/Sovereign Ace.mp3");
    // assign the correct image based on size
    Image img;
    switch (size)
    {
        case 1: {
            img = LoadImage("src/assets/oneTwo.png");
            ImageRotate(&img,90);
            ImageResize(&img, 50, height); 
            sprite = LoadTextureFromImage(img);
            UnloadImage(img);
            break;
        }
        case 2: {
            img = LoadImage("src/assets/oneTwo.png");
            ImageRotate(&img,90);
            ImageResize(&img, 50, height);
            sprite = LoadTextureFromImage(img);
            UnloadImage(img);
            break;
        }
        case 3: {
            img = LoadImage("src/assets/three.png");
            ImageRotate(&img,90);
            ImageResize(&img, 50, height);
            sprite = LoadTextureFromImage(img);
            UnloadImage(img);
            break;
        }
        case 4: {
            img = LoadImage("src/assets/four.png");
            ImageRotate(&img,90);
            ImageResize(&img, 50, height);
            sprite = LoadTextureFromImage(img);
            UnloadImage(img);
            break;
        }
        case 5: {
            img = LoadImage("src/assets/five.png");
            ImageRotate(&img,90);
            ImageResize(&img, 50, height);
            sprite = LoadTextureFromImage(img); 
            UnloadImage(img);
            break;
        }
    }

}

int Ship::getSize() const { return size; }

// draws ship for setup phase vertical version
void Ship::drawShip(Vector2 newPos){
    // make ship rectangle vertical, in case it was horizontal and switches back
    int height = 50*size;
    shipRect = {newPos.x, newPos.y, 50, float(height)};
    // only rotate if direction changed
    if(direction != Direction::Vertical){
        direction = Direction::Vertical;
        Image img = LoadImageFromTexture(sprite);
        ImageRotate(&img,90);
        sprite = LoadTextureFromImage(img);
        UnloadImage(img);
    }
    DrawTextureV(sprite, newPos, WHITE);
    //DrawRectangleLinesEx(shipRect, 1, BLACK);
}
// used during setup phase for horizontal placement
void Ship::drawShipHorizontal(Vector2 newPos){
    //make ship rectangle horizontal, height = 50, width = 50*size
    int width = 50*size;
    shipRect = {newPos.x, newPos.y, float(width), 50};
    // only rotate if direction changed
    if(direction != Direction::Horizontal){
        direction = Direction::Horizontal;
        Image img = LoadImageFromTexture(sprite);
        ImageRotate(&img,-90);
        sprite = LoadTextureFromImage(img);
        UnloadImage(img);
    }
    DrawTextureV(sprite, newPos, WHITE);
    //DrawRectangleLinesEx(shipRect, 1, BLACK);
}

//used for setup phase
bool Ship::shipClicked(){
    if(!isPlaced() && CheckCollisionPointRec(GetMousePosition(),shipRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        return true;
    }
    return false;
}
// for setup phase
void Ship::setShip(const vector<position>& newPositions){
    cout<<"Setting New Ship Positions of size: "<<newPositions.size()<<" should be same size as: "<<size<<endl;
    positions = newPositions;
    //update x and y of rectangle based on where the first position is.
    float newX = 50 + newPositions.at(0).col * 50;
    float newY = 100 + newPositions.at(0).row * 50;
    shipRect.x = newX;
    shipRect.y = newY;
    cout<<"the ships position is now: "<<shipRect.x<<", "<<shipRect.y<<endl;
    cout<<"going in the ";
    if(direction == Direction::Horizontal){cout<<"Horizontal Direction\n";}else{cout<<"Vertical Direction\n";}
}

// draw the ship that has a position set from placement. 
void Ship::draw(){
    Vector2 pos = {shipRect.x, shipRect.y};
    DrawTextureV(sprite, pos, WHITE);
}


bool Ship::isPlaced(){
    if(positions.at(0).row != -1)
        return true;
    return false;
}

void Ship::playAce(){
    PlaySound(Ace);
}

// returns true if there are any ship unit positions that match the given arguments position.
bool Ship::matchingAt(position pos){
    for(int i = 0; i<positions.size(); i++){
        if(positions.at(i).row == pos.row && positions.at(i).col == pos.col){
            return true;
        }
    }
    return false;
}
//should register a hit at the position and set the hit to true;
void Ship::shipHitAt(position pos){
    for(int i = 0; i<positions.size(); i++){
        if(positions.at(i).row == pos.row && positions.at(i).col == pos.col){
            positions.at(i).hit = true;
        }
    }
}

void Ship::drawSunken(){
    //draw ship with low opacity shifted to the right, so it lands on tracking board
    float newX = 550 + shipRect.x;
    Vector2 pos = {newX, shipRect.y};
    DrawTextureV(sprite, pos, {255,255,255,200});
}

bool Ship::isSunk(){
    bool sunk = true;// assume true first and if any are not hit then we set to false.
    for(position& pos: positions){
        if(pos.hit == false)
            return sunk = false;
    }
    return sunk;
}


