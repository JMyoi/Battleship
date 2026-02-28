#include "Player.hpp"
#include "Ship.hpp"

using namespace std;

Player::Player(int shipCount){
    // initialize player ships
    ships.reserve(shipCount);
    for(int i = 0; i < shipCount; i++){
        ships.emplace_back(i + 1);  // Create ship of size (i + 1), 1, 2, 3, ...
    }
    // initialize name
}

int selectedShip = -1;
void Player::drawSetupBoard(){
    // draw the player's board
    Vector2 start = {50, 50};
    playerBoard.Draw(start);

    //draw rectangular ships on the side that can be dragged and dropped onto the board
    Vector2 shipStart = {600, 50};
    for(int i = 0; i < ships.size(); i++){
        if(ships.at(i).shipClicked() || selectedShip == i){ // if a ship is clicked then it should not be displayed but it should follow the cursor.
            selectedShip = i;
        }else{
            //if the ship is not placed then we should display for placement.
            if(!ships.at(i).isPlaced()){
                ships.at(i).drawShip(shipStart);
                shipStart.x += 60; // space out the ships vertically
            }
        }
    }
    // handle click and drag of ships
    /*
    player clicks a ship and the ship pos = mouse cursor
    player clicks on an area on the board and verifies if it can be placed.
    */
    // ship placement on board detection.
    if(selectedShip>=0){
        ships.at(selectedShip).drawShip(GetMousePosition());
        vector<position> newPositions(selectedShip+1);
        if(playerBoard.HandlePlaceShip(selectedShip+1, newPositions)){
            // update the players respective ships position.
            ships.at(selectedShip).setShip(newPositions);
            selectedShip = -1; // deselect after successful placement
        }
    }


}

void Player::drawShipsonBoard(){
    // should draw the ships on the board if there are any ships that are placed on the board.
    //the above funciton should call this one to display ships that are already placed.
}