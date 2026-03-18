#include "Game.hpp"
#include "Board.hpp"
#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// global variables for main menu selections, -1 means nothing selected yet
int selectedOption = -1;     // 0-4  → ship count (1-5)
int selectedMode = -1;       // 0 = 1v1 Local,  1 = vs AI
int selectedDifficulty = -1; // 0 = Easy,  1 = Medium,  2 = Hard  (only used when selectedMode == 1)

//constructor
Game::Game() : player1(1), player2(1), state(GameState::Menu), gameMode(GameMode::LocalPvP), NoOfShips(-1), CurrResult{ShotResult::AlreadyFired} {}

GameState Game::getGameState(){
    return state;
}

void Game::drawMenu() {
    Vector2 mousePos = GetMousePosition();
    int centerX = GetScreenWidth() / 2;

    // --- Title ---
    const char* title = "Battleship";
    DrawText(title, centerX - MeasureText(title, 50) / 2, 70, 50, BLACK);

    // --- Ship count row ---
    const char* shipText = "Select number of ships";
    DrawText(shipText, centerX - MeasureText(shipText, 25) / 2, 150, 25, BLACK);

    // 5 buttons: 50px wide, 60px apart, total span 290px, centered
    vector<Rectangle> shipOptions(5);
    shipOptions[0] = {(float)(centerX - 145), 185, 50, 45};
    for (int i = 1; i < 5; i++)
        shipOptions[i] = {shipOptions[i-1].x + 60, 185, 50, 45};

    getSelectedOption(shipOptions, selectedOption);
    for (int i = 0; i < 5; i++) {
        DrawRectangleRec(shipOptions[i], selectedOption == i ? GRAY : LIGHTGRAY);
        DrawText(to_string(i+1).c_str(), shipOptions[i].x + 15, shipOptions[i].y + 12, 20, BLACK);
    }

    // --- Game mode row ---
    const char* modeText = "Select Game Mode";
    DrawText(modeText, centerX - MeasureText(modeText, 25) / 2, 255, 25, BLACK);

    // [1v1 Local] 120px, gap 20px, [vs AI] 120px — total 260px centered
    vector<Rectangle> modeOptions(2);
    modeOptions[0] = {(float)(centerX - 130), 290, 120, 45}; // 1v1 Local
    modeOptions[1] = {(float)(centerX +  10), 290, 120, 45}; // vs AI

    getSelectedOption(modeOptions, selectedMode);
    DrawRectangleRec(modeOptions[0], selectedMode == 0 ? GRAY : LIGHTGRAY);
    DrawText("1v1 Local", modeOptions[0].x + 10, modeOptions[0].y + 12, 20, BLACK);
    DrawRectangleRec(modeOptions[1], selectedMode == 1 ? GRAY : LIGHTGRAY);
    DrawText("vs AI",     modeOptions[1].x + 30, modeOptions[1].y + 12, 20, BLACK);

    // --- Difficulty row (always drawn; disabled/greyed when 1v1 is selected) ---
    bool aiMode = (selectedMode == 1);
    const char* diffLabel = "Select Difficulty";
    DrawText(diffLabel, centerX - MeasureText(diffLabel, 20) / 2, 360, 20, aiMode ? BLACK : GRAY);

    // [Easy] [Medium] [Hard] — 90px each, 10px gap, total 290px centered
    vector<Rectangle> diffOptions(3);
    diffOptions[0] = {(float)(centerX - 145), 385, 90, 45}; // Easy
    diffOptions[1] = {(float)(centerX -  45), 385, 90, 45}; // Medium
    diffOptions[2] = {(float)(centerX +  55), 385, 90, 45}; // Hard

    if (aiMode) getSelectedOption(diffOptions, selectedDifficulty);

    for (int i = 0; i < 3; i++) {
        Color bg = !aiMode ? Fade(LIGHTGRAY, 0.5f)
                           : (selectedDifficulty == i ? GRAY : LIGHTGRAY);
        DrawRectangleRec(diffOptions[i], bg);
    }
    DrawText("Easy",   diffOptions[0].x + 20, diffOptions[0].y + 12, 20, aiMode ? BLACK : GRAY);
    DrawText("Medium", diffOptions[1].x +  8, diffOptions[1].y + 12, 20, aiMode ? BLACK : GRAY);
    DrawText("Hard",   diffOptions[2].x + 20, diffOptions[2].y + 12, 20, aiMode ? BLACK : GRAY);

    // --- Start button ---
    // Active only when ship count, mode, and (if AI) difficulty are all chosen
    bool canStart = selectedOption != -1 && selectedMode != -1 &&
                    (!aiMode || selectedDifficulty != -1);

    Rectangle startButton = {(float)(centerX - 50), 460, 100, 50};
    DrawRectangleRec(startButton, canStart ? LIGHTGRAY : Fade(LIGHTGRAY, 0.4f));
    DrawText("Start", startButton.x + 20, startButton.y + 15, 20, canStart ? BLACK : GRAY);

    if (canStart && CheckCollisionPointRec(mousePos, startButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        NoOfShips = selectedOption + 1;
        if (selectedMode == 0) {
            gameMode = GameMode::LocalPvP;
        } else {
            if      (selectedDifficulty == 0) gameMode = GameMode::AIEasy;
            else if (selectedDifficulty == 1) gameMode = GameMode::AIMedium;
            else                              gameMode = GameMode::AIHard;
        }
        state = GameState::SetupP1;
        player1 = Player(NoOfShips);
        player2 = Player(NoOfShips);
    }
}




void Game::drawP1Setup(){// draw player 1 setup screen
    const char* p1text = "Player 1, place your ships";
    DrawText(p1text, GetScreenWidth() / 2 - MeasureText(p1text, 30) / 2, 10, 30, BLACK);
    if(player1.drawSetupBoard()){
        if(gameMode == GameMode::LocalPvP){
            state = GameState::P2SetupTransition; // human P2 setup as normal
        } else {
            player2.placeShipsRandomly();          // AI instantly places ships
            state = GameState::P1Transition;       // skip P2 setup entirely
        }
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
        //do nothing, this case happens for the first turn only, 
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
        state = GameState::TurnP1;
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
        state = GameState::TurnP2;
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
    // handle click on tracking board, if it is clicked it will result in true and log the result
    ShotResult result;
    if (player2.drawTrackingBoard(result)) {//if the player made a shot then it will be true and we log if it is a hit or miss, go to the next game state
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
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;
    DrawText(text, centerX - MeasureText(text, 50) / 2, centerY - 100, 50, WHITE);
    //display who wins
    if(player1.checkGameOver()){
        const char* winner = "player 2 Wins";
        DrawText(winner, centerX - MeasureText(winner, 50) / 2, centerY + 50, 50, WHITE);
    }
    else{
        const char* winner = "player 1 Wins";
        DrawText(winner, centerX - MeasureText(winner, 50) / 2, centerY + 50, 50, WHITE);
    }
}



//private helper functions.

// writes the index of the clicked rectangle into `selected`
void Game::getSelectedOption(vector<Rectangle>& options, int& selected){
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        for(int i = 0; i < (int)options.size(); i++){
            if(CheckCollisionPointRec(mousePos, options.at(i))){
                selected = i;
            }
        }
    }
}