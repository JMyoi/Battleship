#include "Player.hpp"

using namespace std;

Player::Player(int shipCount){
    // initialize player board and ships
    ships.reserve(shipCount);
    for(int i = 0; i < shipCount; i++){
        ships.emplace_back(i + 1);  // Create ship of size i+1
    }
}
void Player::drawSetupBoard(){
    // draw the player's board
    Vector2 start = {50, 50};
    Vector2 end = {550, 550};
    playerBoard.Draw(start, end);

    //draw rectangular ships on the side that can be dragged and dropped onto the board
    Vector2 shipStart = {600, 50};
    for(int i = 0; i < ships.size(); i++){
        ships.at(i).drawShip(i, shipStart);
        shipStart.x += 60; // space out the ships vertically
    }
    // handle click and drag of ships
    /*
    if collision of mouse point and rectangle of ship and left mouse is clicked then hover.
    ships rectangle position x and y is set to getMousePosition().
    
    // collision handling of ship. 
    */
    
}