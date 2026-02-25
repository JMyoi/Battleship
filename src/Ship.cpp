#include "Ship.hpp"
#include <vector>
#include <iostream>
using namespace std;



Ship::Ship(int size){
    this->size = size;
    positions.resize(size);
    hitMask.resize(size);
    for(int i = 0; i < size; i++){
        hitMask[i] = false;
    }
}

void Ship::setPosition(vector<position> newPositions){
    positions = newPositions;
}
