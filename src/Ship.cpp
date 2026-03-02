#include "Ship.hpp"
#include <vector>
#include <iostream>
#include <string>
using namespace std;


Ship::Ship(int size){
    this->size = size;
    direction = Direction::Vertical; // by default. 
    int height = 50*size;
    shipRect = {0, 0, 50, float(height)};
    positions.resize(size);
    for(int i = 0; i< positions.size(); i++){
        positions.at(i).hit = false;
        positions.at(i).row = -1;
        positions.at(i).col = -1;
    }

}

// draws ship for setup phase vertical version
void Ship::drawShip(Vector2 newPos){
    // make ship rectangle vertical, in case it was horizontal and switches back 
    int height = 50*size;
    shipRect = {newPos.x, newPos.y, 50, float(height)};
    direction = Direction::Vertical;
    switch (size)
    {
    case 1:
        DrawRectangleRec(shipRect, GRAY);

        break;
    case 2:
        DrawRectangleRec(shipRect, DARKGRAY);
        break;

    case 3:
        DrawRectangleRec(shipRect, BROWN);
        break;
    case 4:
        DrawRectangleRec(shipRect, DARKBROWN);
        break;
    case 5:
        DrawRectangleRec(shipRect, BEIGE);
        break;

    }
    DrawText(to_string(size).c_str(), newPos.x, newPos.y, 20, BLACK);

}

void Ship::drawShipHorizontal(Vector2 newPos){
    //make ship rectangle horizontal, height = 50, width = 50*size
    int width = 50*size;
    shipRect = {newPos.x, newPos.y, float(width), 50};
    direction = Direction::Horizontal;
    switch (size)
    {
    case 1:
        DrawRectangleRec(shipRect, GRAY);

        break;
    case 2:
        DrawRectangleRec(shipRect, DARKGRAY);
        break;

    case 3:
        DrawRectangleRec(shipRect, BROWN);
        break;
    case 4:
        DrawRectangleRec(shipRect, DARKBROWN);
        break;
    case 5:
        DrawRectangleRec(shipRect, BEIGE);
        break;

    }
    DrawText(to_string(size).c_str(), newPos.x, newPos.y, 20, BLACK);
}

bool Ship::shipClicked(){
    if(!isPlaced() && CheckCollisionPointRec(GetMousePosition(),shipRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        return true;
    }
    return false;
}

void Ship::setShip(const vector<position>& newPositions){
    cout<<"Setting New Ship Positions of size: "<<newPositions.size()<<" should be same size as: "<<size<<endl;
    positions = newPositions;
    //update x and y of rectangle based on where the first position is.
    float newX = 50 + newPositions.at(0).col * 50;
    float newY = 50 + newPositions.at(0).row * 50;
    shipRect.x = newX;
    shipRect.y = newY;
    cout<<"the ships position is now: "<<shipRect.x<<", "<<shipRect.y<<endl;
    cout<<"going in the ";
    if(direction == Direction::Horizontal){cout<<"Horizontal Direction\n";}else{cout<<"Vertical Direction\n";}
}

bool Ship::isPlaced(){
    if(positions.at(0).row != -1)
        return true;
    return false;
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
bool Ship::isSunk(){
    bool sunk = true;// assume true first and if any are not hit then we set to false.
    for(position& pos: positions){
        if(pos.hit == false)
            return sunk = false;
    }
    return sunk;
}

void Ship::draw(){
    // draw the ship that has a position set from placement. 
    switch (size)
    {
    case 1:
        DrawRectangleRec(shipRect, GRAY);

        break;
    case 2:
        DrawRectangleRec(shipRect, DARKGRAY);
        break;

    case 3:
        DrawRectangleRec(shipRect, BROWN);
        break;
    case 4:
        DrawRectangleRec(shipRect, DARKBROWN);
        break;
    case 5:
        DrawRectangleRec(shipRect, BEIGE);
        break;

    }
    DrawText(to_string(size).c_str(), shipRect.x, shipRect.y, 20, BLACK);

}

