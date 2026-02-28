#include "Ship.hpp"
#include <vector>
#include <iostream>
using namespace std;


Ship::Ship(int size){
    this->size = size;
    int height = 50*size;
    shipRect = {0, 0, 50, float(height)};
    positions.reserve(size);
    for(int i = 0; i< positions.size(); i++){
        positions.at(i).hit = false;
        positions.at(i).row = -1;
        positions.at(i).col = -1;
    }

}

// draws ship for setup phase
void Ship::drawShip(int i, Vector2 newPos){
    shipRect.x = newPos.x;
    shipRect.y = newPos.y;
    DrawRectangleRec(shipRect, GRAY);
}

void Ship::setPosition(vector<position> newPositions){
    positions = newPositions;
}
