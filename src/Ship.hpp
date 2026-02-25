#pragma once
#include <vector>
using namespace std;

struct position{
    int row;
    int col;
};

class Ship{

    public:
        Ship(int size);
        void setPosition(vector<position> newPositions);
        //void setShipPosition();


    private:
        int size;
        vector<bool> hitMask; // going from right to left or top down
        vector<position> positions;


};