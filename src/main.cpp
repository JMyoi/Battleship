#include "raylib.h"
#include <iostream>
#include "Game.hpp"
using namespace std;



int main(void)
{
    const int screenWidth = 1150;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Battleship");
    InitAudioDevice();             

    //loading background music
    Music BGM = LoadMusicStream("src/assets/AttackonTitansuite.mp3");
    PlayMusicStream(BGM);

    SetTargetFPS(60);
    Game game;

    //background parallax image loading
    Texture2D background = LoadTexture("src/assets/Clouds/Clouds 1/1.png");
    Texture2D midgroundOne = LoadTexture("src/assets/Clouds/Clouds 1/2.png");
    Texture2D midgroundTwo = LoadTexture("src/assets/Clouds/Clouds 1/3.png");
    Texture2D foreground = LoadTexture("src/assets/Clouds/Clouds 1/4.png");

    float scrollingBack = 0.0f;
    float scrollingMidOne = 0.0f;
    float scrollingMidTwo = 0.0f;
    float scrollingFore = 0.0f;
    
    //to scale teh image to fit the height of the screen
    const float layerScale = (float)screenHeight / background.height;
    const float backW = background.width * layerScale;
    const float mid1W = midgroundOne.width * layerScale;
    const float mid2W = midgroundTwo.width * layerScale;
    const float foreW = foreground.width * layerScale;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(BGM);


        scrollingBack -= 0.1f;
        scrollingMidOne -= 0.3f;
        scrollingMidTwo -= 0.5f;
        scrollingFore -= 1.0f;

        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        if (scrollingBack <= -backW) scrollingBack = 0;
        if (scrollingMidOne <= -mid1W) scrollingMidOne = 0;
        if (scrollingMidTwo <= -mid2W) scrollingMidTwo = 0;
        if (scrollingFore <= -foreW) scrollingFore = 0;


        //2. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            // Draw background image twice
            // NOTE: Texture is scaled twice its size
            DrawTextureEx(background, (Vector2){ scrollingBack, 0 }, 0.0f, layerScale, WHITE);
            DrawTextureEx(background, (Vector2){ backW + scrollingBack, 0 }, 0.0f, layerScale, WHITE);

            // Draw first midground image twice
            DrawTextureEx(midgroundOne, (Vector2){ scrollingMidOne, 0 }, 0.0f, layerScale, WHITE);
            DrawTextureEx(midgroundOne, (Vector2){ mid1W + scrollingMidOne, 0 }, 0.0f, layerScale, WHITE);

            // Draw Second midground image twice
            DrawTextureEx(midgroundTwo, (Vector2){ scrollingMidTwo, 0 }, 0.0f, layerScale, WHITE);
            DrawTextureEx(midgroundTwo, (Vector2){ mid2W + scrollingMidTwo, 0 }, 0.0f, layerScale, WHITE);

            // Draw foreground image twice
            DrawTextureEx(foreground, (Vector2){ scrollingFore, 0 }, 0.0f, layerScale, WHITE);
            DrawTextureEx(foreground, (Vector2){ foreW + scrollingFore, 0 }, 0.0f, layerScale, WHITE);
    
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