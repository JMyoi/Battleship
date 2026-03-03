#include "Game.hpp"
#include "Board.hpp"
#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// global variable for main menu Number of ship selection, -1 means no option selected
int selectedOption = -1; 

//constructor
Game::Game() : player1(5), player2(5), state(GameState::Menu), NoOfShips(-1), CurrResult{ShotResult::AlreadyFired} {}

GameState Game::getGameState(){
    return state;
}

void Game::drawMenu() {
    Vector2 mousePos = GetMousePosition();
    int centerX = GetScreenWidth() / 2;

    char title[] = "Battleship";
    DrawText(title, centerX - MeasureText(title, 50) / 2, 100, 50, BLACK);
    char shipSelectionText[] = "Select number of ships";
    DrawText(shipSelectionText, centerX - MeasureText(shipSelectionText, 30) / 2, 230, 30, BLACK);

    // Create 5 rectangle options for number of ships
    // 5 boxes of 50px wide, 10px gap = total width 290px
    vector<Rectangle> options(5);
    options.at(0) = {(float)(centerX - 145), 310, 50, 50};
    for(int i = 1; i<5; i++){
        options.at(i) = {options.at(i-1).x + 60, 310, 50, 50};
    }
    // get and handle number of ships selected
    int selectedOption = getSelectedOption(options);
    // draw the rectangles and text.
    for(int i = 0; i<5; i++){
        if(selectedOption == i){
            DrawRectangleRec(options.at(i), GRAY);
        } else {
            DrawRectangleRec(options.at(i), LIGHTGRAY);
        }
        DrawText(to_string(i+1).c_str(), options.at(i).x + 15, options.at(i).y + 15, 20, BLACK);
    }

    // start button
    Rectangle startButton = {(float)(centerX - 50), 410, 100, 50};
    DrawRectangleRec(startButton, LIGHTGRAY);
    DrawText("Start", startButton.x + 20, startButton.y + 15, 20, BLACK);
    
    // handle start game click will change state to setup and pass NoOfShips to players
    if(CheckCollisionPointRec(mousePos, startButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        if(selectedOption != -1){
            NoOfShips = selectedOption + 1;
            state = GameState::SetupP1;
            player1 = Player(NoOfShips);
            player2 = Player(NoOfShips);
        }
    }
    
}




void Game::drawP1Setup(){
    // draw player 1 setup screen
    const char* p1text = "Player 1, place your ships";
    DrawText(p1text, GetScreenWidth() / 2 - MeasureText(p1text, 30) / 2, 10, 30, BLACK);
    // should update state after the player puts a ship on board.
    if(player1.drawSetupBoard()){
        state = GameState::P2SetupTransition;
    }
}

void Game::drawP2SetupTransition(){
    const char* p2text = "Player 2 Ready for setup?";
    DrawText(p2text, GetScreenWidth() / 2 - MeasureText(p2text, 30) / 2, 10, 30, BLACK);
    //button to click ready.
    int centerX = GetScreenWidth() / 2;
    Rectangle ReadyButton = {(float)(centerX - 50), 410, 100, 50};
    DrawRectangleRec(ReadyButton, LIGHTGRAY);
    DrawText("Ready", ReadyButton.x + 20, ReadyButton.y + 15, 20, BLACK);
    //handle ready click and change state to set up player 2.
    if(CheckCollisionPointRec(GetMousePosition(), ReadyButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        state = GameState::SetupP2;
    }
}

void Game::drawP2Setup(){
    const char* p2text = "Player 2, place your ships";
    DrawText(p2text, GetScreenWidth() / 2 - MeasureText(p2text, 30) / 2, 10, 30, BLACK);
    if(player2.drawSetupBoard()){
        state = GameState::P1Transition;
    }
}

void Game::drawP1Transition(){
    // result from players shot, 
    switch (CurrResult){
    case ShotResult::Hit:
        DrawText("You've Hit!", GetScreenWidth() / 2 - MeasureText("You've Hit!", 30) / 2, 200, 30, GREEN);
        break;
    case ShotResult::Miss:
        DrawText("You've Missed!", GetScreenWidth() / 2 - MeasureText("You've Missed!", 30) / 2, 200, 30, RED);
        break;
    case ShotResult::AlreadyFired:
        //do nothing
        break;   
     }
    const char* text = "Player 1 Ready?";
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, 30) / 2, 300, 30, BLACK);
    // Ready Button
    Rectangle ReadyButton = {(float)((GetScreenWidth() / 2) - 50), 410, 100, 50};
    DrawRectangleRec(ReadyButton, LIGHTGRAY);
    DrawText("Ready", ReadyButton.x + 20, ReadyButton.y + 15, 20, BLACK);
    //handle ready click and change state to player 1 Turn or game over .
    if (CheckCollisionPointRec(GetMousePosition(), ReadyButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    if (player1.checkGameOver()) {   // Player 2 has no remaining ships. Record Player 1 as the winner and transition to GameOver.
        state = GameState::GameOver;
    } else {
        state = GameState::TurnP1;
    }
    }
}

void Game::drawP2Transition(){  
    // result from players shot, 
    switch (CurrResult){
    case ShotResult::Hit:
        DrawText("You've Hit!", GetScreenWidth() / 2 - MeasureText("You've Hit!", 30) / 2, 200, 30, GREEN);
        break;
    case ShotResult::Miss:
        DrawText("You've Missed!", GetScreenWidth() / 2 - MeasureText("You've Missed!", 30) / 2, 200, 30, RED);
        break;
    case ShotResult::AlreadyFired:
        //do nothing
        break;   
     }
    const char* text = "Player 2 Ready?";
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, 30) / 2, 300, 30, BLACK);
     // Ready Button
    Rectangle ReadyButton = {(float)((GetScreenWidth() / 2) - 50), 410, 100, 50};
    DrawRectangleRec(ReadyButton, LIGHTGRAY);
    DrawText("Ready", ReadyButton.x + 20, ReadyButton.y + 15, 20, BLACK);
    //handle ready click and change state to set up player 2 Turn or game over.
    if (CheckCollisionPointRec(GetMousePosition(), ReadyButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    if (player2.checkGameOver()) {   // Player 1 has no remaining ships. Record Player 2 as the winner and transition to GameOver.
        state = GameState::GameOver;
    } else {
        state = GameState::TurnP2;
    }
    }
}

void Game::drawP1Turn(){
    const char* text = "Player 1 Turn";
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, 30) / 2, 10, 30, BLACK);
    const char* Myboard = "My Board";
    DrawText(Myboard,(GetScreenWidth()/4 - MeasureText(Myboard,25)/2), 70,  25, BLACK);
    const char* TargetBoard = "Target Board";
    DrawText(TargetBoard,((GetScreenWidth()/4)*3 - MeasureText(TargetBoard,25)/2), 70,  25, BLACK);
    // render the players board and it's ships, along with hits, misses, sunken ships.
    player1.drawBoard();
    // render tracking board with info about other players board.
    // handle click on tracking board in this trakcing board funciton.
    ShotResult result;
    if (player2.drawTrackingBoard(result)) {
    CurrResult = result;
    // Immediately check if Player 2 has lost after Player 1 fires.
    // If all of Player 2's ships are sunk, transition directly to GameOver
    if (player2.checkGameOver()) {
        state = GameState::GameOver;
    } else {
        state = GameState::P2Transition;
    }
}
}

void Game::drawP2Turn(){
    const char* text = "Player 2 Turn";
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, 30) / 2, 10, 30, BLACK);
    const char* Myboard = "My Board";
    DrawText("MyBoard",(GetScreenWidth()/4 - MeasureText(Myboard,25)/2), 70,  25, BLACK);
    const char* TargetBoard = "Target Board";
    DrawText(TargetBoard,((GetScreenWidth()/4)*3 - MeasureText(TargetBoard,25)/2), 70,  25, BLACK);
    player2.drawBoard();
    ShotResult result;
    if (player1.drawTrackingBoard(result)) {
    CurrResult = result;
    // Immediately check if Player 1 has lost after Player 2 fires.
    // If all of Player 1's ships are sunk, transition directly to GameOver
    if (player1.checkGameOver()) {
        state = GameState::GameOver;
    } else {
        state = GameState::P1Transition;
    }
}
}



void Game::drawGameOver(){
    const char* text = "GameOver";
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, 30) / 2, 10, 30, BLACK);
    //display who wins
    if(player1.checkGameOver()){
        DrawText("player 2 Wins", GetScreenWidth() / 2 - MeasureText("player 2 Wins", 30) / 2, 100, 30, BLACK);
    }
    else{
        DrawText("player 1 Wins", GetScreenWidth() / 2 - MeasureText("player 1 Wins", 30) / 2, 100, 30, BLACK);

    }
}



//private helper functions.


//handles for user click on number of ship option and returns what the user has clicked, 1-5.
int Game::getSelectedOption(vector<Rectangle>& options){
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        for(int i = 0; i < 5; i++){
            if(CheckCollisionPointRec(mousePos, options.at(i))){
                // instead of a global variable could we just use our private data member NoOfShips?
                selectedOption = i;
                return selectedOption;
            }
        }
    }
    return selectedOption;
}