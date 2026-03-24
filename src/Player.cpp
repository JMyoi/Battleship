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
}

int selectedShip = -1; // global varuable for setup, holds what ship is clicked on to be placed on the board.
Direction direction = Direction::Vertical; // global variable to store direction of ship to be placed
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
    //Display r to rotate text
    char RotateText[] = "Press R to Rotate";
    DrawText(RotateText, (GetScreenWidth()/2 + 50), 500, 25, BLACK);

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
        if(playerBoard.HandlePlaceShip(selectedShip+1, newPositions, direction, ErrorMessage)){
            // update the players respective ships position if the placement on board was successfull..
            ships.at(selectedShip).setShip(newPositions);
            selectedShip = -1; // deselect after successful placement
            ErrorMessage = "";
        }
        else{
            //error message from attempted placement
            DrawText(ErrorMessage.c_str(), (GetScreenWidth()/2 + 50), 600, 15, RED);
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
    Vector2 start = {50, 100};
    playerBoard.Draw(start);
    drawShipsonBoard();
    playerBoard.DrawHitsAndMiss(start);

    //draw ratio of how much Ships are sunk
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

    //draw the sunken ships on tracking board and also the ratio of sunken ships.
    int sunkCount = 0;
    for(Ship& ship: ships){
        if(ship.isSunk()){
            sunkCount++;
            ship.drawSunken(); // parameter for fading the ship, tint free
        }
    }
    string sunkText = (to_string(sunkCount) + "/"+ to_string(ships.size())+" Ships Sunk");
    float TileHeight = 50;
    float bottomOfGrid = start.y + 10 * TileHeight;
    DrawText(sunkText.c_str(),
        ((GetScreenWidth()/4)*3 - MeasureText(sunkText.c_str(),25)/2),
        (int)(bottomOfGrid + 45),
        25, BLACK);

    playerBoard.DrawHitsAndMiss(start);// Hits and misses rendered after the board and the sunken ships
    //animation handling 
    playerBoard.DrawExplosion(); // will only draw when active
    playerBoard.DrawMissSplashAnimation();
    if(shotPendingTransition){
        if(!playerBoard.IsShotAnimationActive()){
            res = pendingShotResult;
            shotPendingTransition = false;
            pendingShotResult = ShotResult::AlreadyFired;
            return true; // to change to next game state when the explosion animation is done.
        }
        return false;// to not transition to next game state and wait till animation is done.
    }

    //handle click on tile.
    ShotResult result; // result of the shot will be put here, hit, miss, alreadyFired
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
                    break; // ship hit regestered, no need to interate through other ships.
                }
            }
            pendingShotResult = result;
            shotPendingTransition = true;  
            return false; // pending gate above resolves transition when animation finishes.
        }
        else if(result == ShotResult::Miss){
            cout<<"You've Missed at: "<<pos.row<<", "<<pos.col<<endl;
            pendingShotResult = result;
            shotPendingTransition = true;
            return false;
        } 
        else if(result == ShotResult::AlreadyFired){
            cout<<"You've Already Fired at: "<<pos.row<<", "<<pos.col<<endl;
            return false;
        }
    }
    return false;
    
}

void Player::placeShipsRandomly(){
    for(int i = 0; i < (int)ships.size(); i++){
        int shipSize = ships.at(i).getSize();
        string errMsg;
        bool placed = false;
        while(!placed){
            Direction dir = (GetRandomValue(0, 1) == 0) ? Direction::Vertical : Direction::Horizontal;
            int row = GetRandomValue(0, 9);
            int col = GetRandomValue(0, 9);
            vector<position> newPositions(shipSize);
            if(playerBoard.PlaceShipAt(row, col, shipSize, newPositions, dir, errMsg)){
                ships.at(i).setShip(newPositions);
                ships.at(i).setDirection(dir); // sync sprite orientation with placement direction
                placed = true;
            }
        }
    }
}

void Player::startReceivingFire(int row, int col, ShotResult& result){
    position at;
    playerBoard.FireAt(row, col, result, at);
    if(result == ShotResult::Hit){
        for(Ship& ship: ships){
            if(ship.matchingAt(at)){
                ship.shipHitAt(at);
                if(ship.isSunk()) ship.playAce();
                break;
            }
        }
    }
    shotPendingTransition = true;
    pendingShotResult = result;
}

bool Player::drawBoardAITurn(){
    Vector2 start = {50, 100};
    playerBoard.Draw(start);
    drawShipsonBoard();
    playerBoard.UpdateAnimations();
    playerBoard.DrawHitsAndMiss(start);
    playerBoard.DrawExplosion();
    playerBoard.DrawMissSplashAnimation();

    int sunkCount = 0;
    for(Ship& ship: ships){
        if(ship.isSunk()) sunkCount++;
    }
    string sunkText = to_string(sunkCount) + "/" + to_string(ships.size()) + " Ships Sunk";
    float bottomOfGrid = start.y + 10 * 50;
    DrawText(sunkText.c_str(), (GetScreenWidth()/4 - MeasureText(sunkText.c_str(), 25)/2), (int)(bottomOfGrid + 45), 25, BLACK);

    if(shotPendingTransition){
        if(!playerBoard.IsShotAnimationActive()){
            shotPendingTransition = false;
            return true;
        }
        return false;
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
    for(int i = 0; i<ships.size(); i++){
        if(ships.at(i).isPlaced()){
            ships.at(i).draw();
        }
    }
}