#include "raylib.h"
#include <iostream>
#include "Game.hpp"
using namespace std;


int main(void)
{
    const int screenWidth = 1150;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Battleship");

    SetTargetFPS(60);
    Game game;
    
    while (!WindowShouldClose())
    {
        //1, Event handling and update
        //2. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            switch(GameState state = game.getGameState()){
                case GameState::Menu:
                    game.drawMenu();
                    break;
                case GameState::SetupP1:
                    game.drawP1Setup();
                    break;
                case GameState::P2Transition:
                    game.drawP2Transition();
                    break;
            }


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        

    return 0;
}