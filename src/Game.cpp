#include "Game.hpp"
#include "raylib.h"
#include "vector"
using namespace std;

int selectedOption = -1;

int getSelectedOption(vector<Rectangle>& options){
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        for(int i = 0; i < 5; i++){
            if(CheckCollisionPointRec(mousePos, options.at(i))){
                selectedOption = i;
                return selectedOption;
            }
        }
    }
    return selectedOption;

}

Game::Game() : player1(5), player2(5), state(GameState::Menu), NoOfShips(5) {
    // Initialize game state, players, and any other necessary components

}

void Game::drawMenu() {

    Vector2 mousePos = GetMousePosition();

    char title[] = "Battleship";
    DrawText(title, 350, 100, 50, BLACK);
    char shipSelectionText[] = "Select number of ships";
    DrawText(shipSelectionText, 310, 200, 30, BLACK);

    // draw rectangle options for number of ships
    vector<Rectangle> options(5);
    options.at(0) = {350, 250, 50, 50};
    for(int i = 1; i<5; i++){
        options.at(i) = {options.at(i-1).x + 60, 250, 50, 50};
    }

     int selectedOption = getSelectedOption(options);

    for(int i = 0; i<5; i++){
        if(selectedOption == i){
            DrawRectangleRec(options.at(i), GRAY);
        } else {
            DrawRectangleRec(options.at(i), LIGHTGRAY);
        }
        DrawText(to_string(i+1).c_str(), options.at(i).x + 15, options.at(i).y + 10, 20, BLACK);
    }

    // start button
    Rectangle startButton = {450, 350, 100, 50};
    if(CheckCollisionPointRec(mousePos, startButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        if(selectedOption != -1){
            NoOfShips = selectedOption + 1;
            // Handle selected option here
        }
    }
    DrawRectangleRec(startButton, LIGHTGRAY);
    DrawText("Start", startButton.x + 20, startButton.y + 10, 20, BLACK);
    // handle start game click will change state to setup and pass NoOfShips to players
    if(CheckCollisionPointRec(mousePos, startButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        if(selectedOption != -1){
            state = GameState::SetupP1;
            player1 = Player(NoOfShips);
            player2 = Player(NoOfShips);
            // Handle selected option here
        }
    }

}

void Game::updateGameState(GameState newState){
    state = newState;
    drawSetup();
}
GameState Game::getGameState(){
    return state;
}

void Game::drawSetup(){
    // draw setup screen based on state
    // draw player 1 setup screen
    char p1Setup[] = "Player 1, place your ships";
    DrawText(p1Setup, 310, 10, 30, BLACK);
    player1.drawSetupBoard();
        
}


