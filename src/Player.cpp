#include "Player.hpp"

using namespace std;

Player::Player(int shipCount){
    // initialize player board and ships
    ships.reserve(shipCount);
    for(int i = 0; i < shipCount; i++){
        ships.emplace_back(i + 1);  // Create ship of size i+1
    }
}