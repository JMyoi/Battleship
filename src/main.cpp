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

    // Texture2D Explosion = LoadTexture("src/assets/Explosion.png");
    // Vector2 position = {0,0};
    // Rectangle frameRec = {0, 0, static_cast<float>(Explosion.width/7)-12, static_cast<float>(Explosion.height)};//-20 because it's leaking a bit to the left
    // int currentFrame = 0;
    // int frameCounter = 0;
    // int framesSpeed = 8;
    // bool active = false;

   

    
    while (!WindowShouldClose())
    {
        //1, Event handling and update
    //    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !active)
    //     {
    //         position = GetMousePosition();
    //         active = true;

    //         // center using ONE frame size (not full spritesheet size)
    //         position.x -= frameRec.width / 2.0f;
    //         position.y -= frameRec.height / 2.0f;

    //         currentFrame  = 0;
    //         frameCounter = 0;
    //         // active = true;   // redundant, can remove
    //     }
    //     if(active){
    //         frameCounter++;
    //         if(frameCounter >=(60/framesSpeed)){
    //             frameCounter = 0;
    //             currentFrame++;
    
    //             if(currentFrame>6) {
    //                 currentFrame= 0;
    //                 active = false;
    //             }
    
    //             frameRec.x = (static_cast<float>(currentFrame)) * (static_cast<float>(Explosion.width/7)-12);
    //         }

    //     }

        //2. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            // if(active)
            //     DrawTextureRec(Explosion, frameRec, position, WHITE);
    

            switch(GameState state = game.getGameState()){

                case GameState::Menu:
                    game.drawMenu();
                    break;
                case GameState::SetupP1:
                    game.drawP1Setup();
                    break;
                case GameState::P2SetupTransition:
                    game.drawP2SetupTransition();
                    break;
                case GameState::SetupP2:
                    game.drawP2Setup();
                    break;
                case GameState::P1Transition:
                    game.drawP1Transition();
                    break;
                case GameState::P2Transition:
                    game.drawP2Transition();
                    break;
                case GameState::TurnP1:
                    game.drawP1Turn();
                    break;
                case GameState::TurnP2:
                    game.drawP2Turn();
                    break;
                case GameState::GameOver:
                    game.drawGameOver();
                    break;
            }


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        

    return 0;
}