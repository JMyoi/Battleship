#include "Game.hpp"
#include "raylib.h"
#include <vector>
#include <string>
using namespace std;

// global variable for main menu Number of ship selection, -1 means no option selected
int selectedOption = -1; 

//constructor
Game::Game() : player1(5), player2(5), state(GameState::Menu), NoOfShips(-1) {}


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


GameState Game::getGameState(){
    return state;
}

void Game::drawP1Setup(){
    // draw player 1 setup screen
    const char* p1text = "Player 1, place your ships";
    DrawText(p1text, GetScreenWidth() / 2 - MeasureText(p1text, 30) / 2, 10, 30, BLACK);
    player1.drawSetupBoard();
    
}

//private helper functions.

void Game::updateGameState(GameState newState){
    state = newState;
}

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