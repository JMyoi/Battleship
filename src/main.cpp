#include "raylib.h"
#include <iostream>
#include "Game.hpp"
using namespace std;

void DrawParallaxBackground(Texture2D background, Texture2D midgroundOne, Texture2D midgroundTwo, Texture2D foreground,
                            float& scrollingBack, float& scrollingMidOne, float& scrollingMidTwo, float& scrollingFore,
                            int screenHeight);

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
    Game game; // main game object

    //Main Menu backgrounds
    Texture2D MMbackground = LoadTexture("src/assets/Clouds/Clouds 1/1.png");
    Texture2D MMmidgroundOne = LoadTexture("src/assets/Clouds/Clouds 1/2.png");
    Texture2D MMmidgroundTwo = LoadTexture("src/assets/Clouds/Clouds 1/3.png");
    Texture2D MMforeground = LoadTexture("src/assets/Clouds/Clouds 1/4.png");
    //Player 1 background
    Texture2D P1background = LoadTexture("src/assets/Clouds/Clouds 2/1.png");
    Texture2D P1midgroundOne = LoadTexture("src/assets/Clouds/Clouds 2/2.png");
    Texture2D P1midgroundTwo = LoadTexture("src/assets/Clouds/Clouds 2/3.png");
    Texture2D P1foreground = LoadTexture("src/assets/Clouds/Clouds 2/4.png");
    //player 2 background
    Texture2D P2background = LoadTexture("src/assets/Clouds/Clouds 4/1.png");
    Texture2D P2midgroundOne = LoadTexture("src/assets/Clouds/Clouds 4/2.png");
    Texture2D P2midgroundTwo = LoadTexture("src/assets/Clouds/Clouds 4/3.png");
    Texture2D P2foreground = LoadTexture("src/assets/Clouds/Clouds 4/4.png");
    //gameover
    Texture2D GGbackground = LoadTexture("src/assets/Clouds/Clouds 3/1.png");
    Texture2D GGmidgroundOne = LoadTexture("src/assets/Clouds/Clouds 3/2.png");
    Texture2D GGmidgroundTwo = LoadTexture("src/assets/Clouds/Clouds 3/3.png");
    Texture2D GGforeground = LoadTexture("src/assets/Clouds/Clouds 3/4.png");

    float scrollingBack = 0.0f;
    float scrollingMidOne = 0.0f;
    float scrollingMidTwo = 0.0f;
    float scrollingFore = 0.0f;
    

    while (!WindowShouldClose())
    {
        UpdateMusicStream(BGM);
        BeginDrawing();
            ClearBackground(RAYWHITE);
    
            switch(GameState state = game.getGameState()){
                case GameState::Menu:
                    DrawParallaxBackground(MMbackground, MMmidgroundOne, MMmidgroundTwo, MMforeground,
                           scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                           screenHeight);
                    game.drawMenu();
                    break;
                case GameState::SetupP1:
                    DrawParallaxBackground(P1background, P1midgroundOne, P1midgroundTwo, P1foreground,
                        scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                        screenHeight);
                    game.drawP1Setup();
                    break;
                case GameState::P2SetupTransition:
                    DrawParallaxBackground(P2background, P2midgroundOne, P2midgroundTwo, P2foreground,
                            scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                            screenHeight);
                    game.drawP2SetupTransition();
                    break;
                case GameState::SetupP2:
                    DrawParallaxBackground(P2background, P2midgroundOne, P2midgroundTwo, P2foreground,
                            scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                            screenHeight);
                    game.drawP2Setup();
                    break;
                case GameState::P1Transition:
                    DrawParallaxBackground(P1background, P1midgroundOne, P1midgroundTwo, P1foreground,
                           scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                           screenHeight);
                    game.drawP1Transition();
                    break;
                case GameState::P2Transition:
                        DrawParallaxBackground(P2background, P2midgroundOne, P2midgroundTwo, P2foreground,
                            scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                            screenHeight);
                    game.drawP2Transition();
                    break;
                case GameState::TurnP1:
                    DrawParallaxBackground(P1background, P1midgroundOne, P1midgroundTwo, P1foreground,
                           scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                           screenHeight);
                    game.drawP1Turn();
                    break;
                case GameState::TurnP2:
                        DrawParallaxBackground(P2background, P2midgroundOne, P2midgroundTwo, P2foreground,
                            scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                            screenHeight);
                    game.drawP2Turn();
                    break;
                case GameState::AITurn:
                    DrawParallaxBackground(P1background, P1midgroundOne, P1midgroundTwo, P1foreground,
                           scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                           screenHeight);
                    game.drawAITurn();
                    break;
                case GameState::GameOver:
                        DrawParallaxBackground(GGbackground, GGmidgroundOne, GGmidgroundTwo, GGforeground,
                            scrollingBack, scrollingMidOne, scrollingMidTwo, scrollingFore,
                            screenHeight);
                    game.drawGameOver();
                    break;
            }
        EndDrawing();
    }

    // De-Initialization
    Tile::UnloadSharedTextures();

    UnloadTexture(MMbackground);
    UnloadTexture(MMmidgroundOne);
    UnloadTexture(MMmidgroundTwo);
    UnloadTexture(MMforeground);

    UnloadTexture(P1background);
    UnloadTexture(P1midgroundOne);
    UnloadTexture(P1midgroundTwo);
    UnloadTexture(P1foreground);

    UnloadTexture(P2background);
    UnloadTexture(P2midgroundOne);
    UnloadTexture(P2midgroundTwo);
    UnloadTexture(P2foreground);

    UnloadTexture(GGbackground);
    UnloadTexture(GGmidgroundOne);
    UnloadTexture(GGmidgroundTwo);
    UnloadTexture(GGforeground);

    UnloadMusicStream(BGM);
    CloseAudioDevice();
    CloseWindow();        

    return 0;
}

void DrawParallaxBackground(Texture2D background, Texture2D midgroundOne, Texture2D midgroundTwo, Texture2D foreground,
                            float& scrollingBack, float& scrollingMidOne, float& scrollingMidTwo, float& scrollingFore,
                            int screenHeight) {
    const float layerScale = (float)screenHeight / background.height;
    const float backW = background.width * layerScale;
    const float mid1W = midgroundOne.width * layerScale;
    const float mid2W = midgroundTwo.width * layerScale;
    const float foreW = foreground.width * layerScale;

    // Update scrolling positions
    scrollingBack -= 0.1f;
    scrollingMidOne -= 0.3f;
    scrollingMidTwo -= 0.5f;
    scrollingFore -= 1.0f;

    // Wrap around when fully scrolled
    if (scrollingBack <= -backW) scrollingBack = 0;
    if (scrollingMidOne <= -mid1W) scrollingMidOne = 0;
    if (scrollingMidTwo <= -mid2W) scrollingMidTwo = 0;
    if (scrollingFore <= -foreW) scrollingFore = 0;

    // Draw all layers
    DrawTextureEx(background, (Vector2){ scrollingBack, 0 }, 0.0f, layerScale, WHITE);
    DrawTextureEx(background, (Vector2){ backW + scrollingBack, 0 }, 0.0f, layerScale, WHITE);

    DrawTextureEx(midgroundOne, (Vector2){ scrollingMidOne, 0 }, 0.0f, layerScale, WHITE);
    DrawTextureEx(midgroundOne, (Vector2){ mid1W + scrollingMidOne, 0 }, 0.0f, layerScale, WHITE);

    DrawTextureEx(midgroundTwo, (Vector2){ scrollingMidTwo, 0 }, 0.0f, layerScale, WHITE);
    DrawTextureEx(midgroundTwo, (Vector2){ mid2W + scrollingMidTwo, 0 }, 0.0f, layerScale, WHITE);

    DrawTextureEx(foreground, (Vector2){ scrollingFore, 0 }, 0.0f, layerScale, WHITE);
    DrawTextureEx(foreground, (Vector2){ foreW + scrollingFore, 0 }, 0.0f, layerScale, WHITE);
}