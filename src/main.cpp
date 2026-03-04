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
    //Game game;

    
    Texture2D drip = LoadTexture("src/assets/poofDrip.png");
    SetTextureFilter(drip, TEXTURE_FILTER_POINT);

    
    Vector2 position = {0,0};
    
    const int NumFrames = 6;
    Rectangle dripFrames[NumFrames] = {
        {0, 0, 200, static_cast<float>(drip.height)},
        {200, 0, 340, static_cast<float>(drip.height)},
        {500, 0, 340, static_cast<float>(drip.height)},
        {850, 0, 340, static_cast<float>(drip.height)},
        {1220, 0, 340, static_cast<float>(drip.height)},
        {1600, 0, 400, static_cast<float>(drip.height)}
    };
    cout<<"texture width: "<<drip.width<<", frame count: "<<NumFrames<<endl;
    Rectangle frameRec = dripFrames[0];
    int currentFrame = 0;
    int frameCounter = 0;
    int framesSpeed = 12;
    bool active = false;

    float scale = 0.4f; // 40% size
    Rectangle destRec = {
        position.x,
        position.y,
        frameRec.width * scale,
        frameRec.height * scale
    };

    
    while (!WindowShouldClose())
    {
        //1, Event handling and update
       if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !active)
        {
            position = GetMousePosition();
            frameRec = dripFrames[0];
            // center using ONE frame size (not full spritesheet size)
            position.x -= (frameRec.width * scale) / 2.0f;
            position.y -= (frameRec.height * scale) / 2.0f; 
            
            currentFrame  = 0;
            frameCounter = 0;
            active = true;
        }

        if (active) {
            frameCounter++;
            if (frameCounter >= (60 / framesSpeed)) {
                frameCounter = 0;
                currentFrame++;

                if (currentFrame >= NumFrames) {
                    currentFrame = 0;
                    active = false;
                }

                frameRec = dripFrames[currentFrame];
            }

            destRec.x = position.x;
            destRec.y = position.y;
            destRec.width = frameRec.width * scale;
            destRec.height = frameRec.height * scale;
        }

        //2. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            if(active)
                DrawTexturePro(
                    drip,
                    frameRec,        // source frame from sheet
                    destRec,         // scaled destination
                    {0, 0},          // origin
                    0.0f,            // rotation
                    WHITE
                );
    

            // switch(GameState state = game.getGameState()){

            //     case GameState::Menu:
            //         game.drawMenu();
            //         break;
            //     case GameState::SetupP1:
            //         game.drawP1Setup();
            //         break;
            //     case GameState::P2SetupTransition:
            //         game.drawP2SetupTransition();
            //         break;
            //     case GameState::SetupP2:
            //         game.drawP2Setup();
            //         break;
            //     case GameState::P1Transition:
            //         game.drawP1Transition();
            //         break;
            //     case GameState::P2Transition:
            //         game.drawP2Transition();
            //         break;
            //     case GameState::TurnP1:
            //         game.drawP1Turn();
            //         break;
            //     case GameState::TurnP2:
            //         game.drawP2Turn();
            //         break;
            //     case GameState::GameOver:
            //         game.drawGameOver();
            //         break;
            // }


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        

    return 0;
}