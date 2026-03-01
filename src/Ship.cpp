#include "Ship.hpp"
#include <vector>
#include <iostream>
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
    // shipRect.x = newPos.x;
    // shipRect.y = newPos.y;
    direction = Direction::Vertical;
    DrawRectangleRec(shipRect, GRAY);
}

void Ship::drawShipHorizontal(Vector2 newPos){
    //make ship rectangle horizontal, height = 50, width = 50*size
    int width = 50*size;
    shipRect = {newPos.x, newPos.y, float(width), 50};
    direction = Direction::Horizontal;
    // shipRect.x = newPos.x;
    // shipRect.y = newPos.y;
    DrawRectangleRec(shipRect, GRAY);

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

void Ship::draw(){
    // draw the ship that has a position set from placement. 
    DrawRectangleRec(shipRect, GRAY);
}

