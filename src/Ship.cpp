#include "Ship.hpp"
#include <vector>
#include <iostream>
using namespace std;



Ship::Ship(int size){
    this->size = size;
    positions.resize(size);
    shipSprite.resize(size);
    for(int i = 0; i<size; i++){
        shipSprite.at(i) = {0, 0, 50, 50};
    }
    hitMask.resize(size);
    for(int i = 0; i < size; i++){
        hitMask[i] = false;
    }
}
void Ship::drawShip(int i, Vector2 newPos){
    for(int j = 0; j<size; j++){
        shipSprite.at(j).x = newPos.x;
        shipSprite.at(j).y = newPos.y + j*50; // render the length of the ship vertically
        DrawRectangleRec(shipSprite.at(j), GRAY);
    }
}

void Ship::setPosition(vector<position> newPositions){
    positions = newPositions;
}
