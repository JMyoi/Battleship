#include "Player.hpp"
#include "Ship.hpp"
#include "Board.hpp"
#include <iostream>
#include <string>

using namespace std;

Player::Player(int shipCount){
    // initialize player ships
    ships.reserve(shipCount);
    for(int i = 0; i < shipCount; i++){
        ships.emplace_back(i + 1);  // Create ship of size (i + 1), 1, 2, 3, ...
    }
    shotPendingTransition = false;
    pendingShotResult = ShotResult::AlreadyFired;
    // initialize name
}

int selectedShip = -1; // global varuable for setup, holds what ship is clicked on to be placed on the board.
Direction direction = Direction::Vertical;
string ErrorMessage = "";// might need to be placed as a global variable because every frame this is reassigned to empty
// draws the players setup board, logic for handling ship placement and ship displaying on board and out of board for seleciton and placement.
bool Player::drawSetupBoard(){
    // draw the player's board and the ships on the board if there are any placed before.
    Vector2 start = {50, 100};
    playerBoard.Draw(start);
    drawShipsonBoard();

    //draw rectangular ships on the side that can be dragged and dropped onto the board
    Vector2 shipStart = {600, 100};
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
        //Logic for Rotating ship with R and displaying the right orientation, based on direction.
        if(IsKeyPressed(KEY_R)){
            direction = (direction == Direction::Vertical) ? Direction::Horizontal : Direction::Vertical ;
            cout<<"Direction Changed "<<endl;
        }
        if(direction == Direction::Vertical){
            ships.at(selectedShip).drawShip({GetMousePosition().x - 25, GetMousePosition().y - 25});
        }
        else if(direction == Direction::Horizontal){
            ships.at(selectedShip).drawShipHorizontal({GetMousePosition().x - 25, GetMousePosition().y - 25});
        }
        vector<position> newPositions(selectedShip+1);
        if(playerBoard.HandlePlaceShip(selectedShip+1, newPositions, direction, ErrorMessage)){// should pass in Direction as another parameter
            // update the players respective ships position if the placement on board was successfull..
            ships.at(selectedShip).setShip(newPositions);
            selectedShip = -1; // deselect after successful placement
            ErrorMessage = "";
        }
        else{
            //error message from attempted placement
            DrawText(ErrorMessage.c_str(), (GetScreenWidth()/2 + 50), 600, 20, RED);
        }
    }

    //Display r to rotate text
    char RotateText[] = "Press R to Rotate";
    DrawText(RotateText, (GetScreenWidth()/2 + 50), 500, 25, BLACK);

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
    Vector2 start = {50, 100};
    playerBoard.Draw(start);
    drawShipsonBoard();
    playerBoard.DrawHitsAndMiss(start);
    //draw how much Ships are sunk
    int sunkCount = 0;
    for(Ship& ship: ships){
        if(ship.isSunk())
            sunkCount++;
    }
    string sunkText = (to_string(sunkCount) + "/"+ to_string(ships.size())+" Ships Sunk");
    float TileHeight = 50;
    float bottomOfGrid = start.y + 10 * TileHeight;
    DrawText(sunkText.c_str(),
         (GetScreenWidth()/4 - MeasureText(sunkText.c_str(),25)/2),
         (int)(bottomOfGrid + 45),   // under the letter
         25, BLACK);
}

//should return true if hit or miss is registered and update the result argument so that the game can change it's state and have information if the player hit or miss.
bool Player::drawTrackingBoard(ShotResult& res){
    Vector2 start = {600, 100};
    playerBoard.Draw(start);
    playerBoard.UpdateAnimations();
    //draw how much ships are sunk
    int sunkCount = 0;
    for(Ship& ship: ships){
        if(ship.isSunk()){
            sunkCount++;
            ship.drawSunken(); // parameter for fading the ship, tint fre
        }
    }
    playerBoard.DrawHitsAndMiss(start);// should draw here and be rendered last after the board and the sunken ships
    playerBoard.DrawExplosion(); // will only draw when active
    playerBoard.DrawMissSplashAnimation();
    string sunkText = (to_string(sunkCount) + "/"+ to_string(ships.size())+" Ships Sunk");
    float TileHeight = 50;
    float bottomOfGrid = start.y + 10 * TileHeight;
    DrawText(sunkText.c_str(),
    ((GetScreenWidth()/4)*3 - MeasureText(sunkText.c_str(),25)/2),
    (int)(bottomOfGrid + 45),
    25, BLACK);

    if(shotPendingTransition){
        if(!playerBoard.IsShotAnimationActive()){
            res = pendingShotResult;
            shotPendingTransition = false;
            pendingShotResult = ShotResult::AlreadyFired;
            return true;
        }
        return false;
    }

    //handle click on tile.
    ShotResult result; // result will be put here, hit, miss, alreadyFired
    position pos; // position at the fire attempt will be stored here, row, col, hit.
    if(playerBoard.HandleFire(result, pos)){
        if(result == ShotResult::Hit){
            cout<<"You've hit the ship at: "<<pos.row<<", "<<pos.col<<endl;
            // register the hit on the respective ship segment
            for(Ship& ship: ships){
                if(ship.matchingAt(pos)){
                    ship.shipHitAt(pos);
                    // if that hit made the ship sink then play the ace sound.
                    if(ship.isSunk()){
                        ship.playAce();
                    }
                    
                }
            }
            pendingShotResult = result;
            shotPendingTransition = true;
            if(!playerBoard.IsShotAnimationActive()){
                res = pendingShotResult;
                shotPendingTransition = false;
                pendingShotResult = ShotResult::AlreadyFired;
                return true; // to change to next game state when the explosion animation is done.
            }
            return false; // to not transition to next game state and wait till animation is done.
        }
        else if(result == ShotResult::Miss){
            cout<<"You've Missed at: "<<pos.row<<", "<<pos.col<<endl;
            pendingShotResult = result;
            shotPendingTransition = true;
            if(!playerBoard.IsShotAnimationActive()){
                res = pendingShotResult;
                shotPendingTransition = false;
                pendingShotResult = ShotResult::AlreadyFired;
                return true;
            }
            return false;
        } 
        else if(result == ShotResult::AlreadyFired){
            cout<<"You've Already Fired at: "<<pos.row<<", "<<pos.col<<endl;
            return false;
        }
    }
    return false;
    
}

bool Player::checkGameOver(){
    //check if all ships are sunk
    bool allSunk = true;// assume true first
    for(Ship& ship: ships){
        if(!ship.isSunk()){
            return allSunk = false;
        }
    }
    return allSunk;
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