#include "raylib.h"
#include <iostream>
#include "Game.hpp"
using namespace std;


int main(void)
{
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Battleship");
    SetTargetFPS(60);
    Game game;
    
    while (!WindowShouldClose())
    {
        //1, Event handling

        //2. update

        //3. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(GameState state = game.getGameState()){
                case GameState::Menu:
                    game.drawMenu();
                    break;
                case GameState::SetupP1:
                    game.drawSetup();
                    break;
            }


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        

    return 0;
}