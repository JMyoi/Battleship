#include "Player.hpp"
#include "Ship.hpp"
#include "Board.hpp"
#include <iostream>

using namespace std;

Player::Player(int shipCount){
    // initialize player ships
    ships.reserve(shipCount);
    for(int i = 0; i < shipCount; i++){
        ships.emplace_back(i + 1);  // Create ship of size (i + 1), 1, 2, 3, ...
    }
    // initialize name
}

int selectedShip = -1; // global varuable for setup, holds what ship is clicked on to be placed on the board.
Direction direction = Direction::Vertical;
// draws the players setup board, logic for handling ship placement and ship displaying on board and out of board for seleciton and placement.
bool Player::drawSetupBoard(){
    // draw the player's board and the ships on the board if there are any placed before.
    Vector2 start = {50, 50};
    playerBoard.Draw(start);
    drawShipsonBoard();

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
    // handle click and drag of ships ship placement on board detection.
    if(selectedShip>=0){
        //TODO: Logic for Rotating ship with R and displaying the right orientation, based on direction.
        if(IsKeyPressed(KEY_R)){
            direction = (direction == Direction::Vertical) ? Direction::Horizontal : Direction::Vertical ;
            cout<<"Direction Changed "<<endl;
        }
        if(direction == Direction::Vertical){
            ships.at(selectedShip).drawShip(GetMousePosition());
        }
        else if(direction == Direction::Horizontal){
            ships.at(selectedShip).drawShipHorizontal(GetMousePosition());
        }
        
        vector<position> newPositions(selectedShip+1);
        if(playerBoard.HandlePlaceShip(selectedShip+1, newPositions, direction)){// should pass in Direction as another parameter
            // update the players respective ships position if the placement on board was successfull..
            ships.at(selectedShip).setShip(newPositions);
            selectedShip = -1; // deselect after successful placement
        }
    }

    //display ready button
    int centerX = GetScreenWidth()/2;
    // Ready button
    Rectangle startButton = {(float)(centerX - 50), 675, 100, 50};
    DrawRectangleRec(startButton, LIGHTGRAY);
    DrawText("Ready", startButton.x + 20, startButton.y + 15, 20, BLACK);
    //handle ready click, should only return true if all ships are placed on board.
    if(CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        bool allPlaced = true;
        for(int i = 0; i<ships.size(); i++){
            if(!(ships.at(i).isPlaced())){
                allPlaced = false;
                break;
            }
        }
        return allPlaced;
    }
    return false;
}

void Player::drawBoard(){
    Vector2 start = {50, 50};
    playerBoard.Draw(start);
    drawShipsonBoard();
}

//should return true if hit or miss is registered and update the result argument so that the game can change it's state and have information if the player hit or miss.
bool Player::drawTrackingBoard(ShotResult& res){
    Vector2 start = {600, 50};
    playerBoard.Draw(start);
    //handle click on tile.
    ShotResult result; // result will be put here, hit, miss, alreadyFired
    position pos; // position at the fire attempt will be stored here, row, col, hit.
    if(playerBoard.HandleFire(result, pos)){
        if(result == ShotResult::Hit){
            cout<<"You've hit the ship at: "<<pos.row<<", "<<pos.col<<endl;
            // register the hit on the respective ship segment
            for(Ship ship: ships){
                if(ship.matchingAt(pos)){
                    ship.shipHitAt(pos);
                }
            }
            //change state to next player transition screen
            res = result; // res = Hit
            return true;
        }
        else if(result == ShotResult::Miss){
            cout<<"You've Missed at: "<<pos.row<<", "<<pos.col<<endl;
            //change state to next player transition screen.
            res = result; // result is miss
            return true;
        } 
        else if(result == ShotResult::AlreadyFired){
            cout<<"You've Already Fired at: "<<pos.row<<", "<<pos.col<<endl;
            return false;
        }
    }
    return false;
    
}

//helper funcitons.

void Player::drawShipsonBoard(){
    // should draw the ships on the board if there are any ships that are placed on the board.
    //the above funciton should call this one to display ships that are already placed.
    for(int i = 0; i<ships.size(); i++){
        if(ships.at(i).isPlaced()){
            ships.at(i).draw();
        }
    }
}