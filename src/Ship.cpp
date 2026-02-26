#include "Ship.hpp"
#include <vector>
#include <iostream>
using namespace std;



Ship::Ship(int size){
    this->size = size;
    positions.resize(size);
    int height = 50*size;
    shipRect = {0, 0, 50, float(height)};
    hitMask.resize(size);
    for(int i = 0; i < size; i++){
        hitMask[i] = false;
    }
}

void Ship::drawShip(int i, Vector2 newPos){
    shipRect.x = newPos.x;
    shipRect.y = newPos.y;
    DrawRectangleRec(shipRect, GRAY);
}

void Ship::setPosition(vector<position> newPositions){
    positions = newPositions;
}
