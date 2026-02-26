#include "raylib.h"
#include <iostream>
using namespace std;

int main(void)
{
    // Initialization

    InitWindow(1920, 1080, "Battleship");
    const char *ImagePath = "./src/CNY.png";
    //Texture2D CNY = LoadTexture(ImagePath);
    Image CNYImage = LoadImage(ImagePath);
    Image* C = new Image(CNYImage);
    ImageResize(&CNYImage, 200, 200); // we could just pass the &CNYImage by address, instead of makking another variable in heap.
    Texture2D CNYImg = LoadTextureFromImage(CNYImage);

    SetTargetFPS(60);              

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //Event handling
        // Update, variables, objects, position.
        cout<<IsKeyDown(KEY_A)<<endl;
       
        // Draw the udpated outputs to screen.
        BeginDrawing();
            DrawFPS(10, 10);
            ClearBackground(RAYWHITE);
            DrawTexture(CNYImg, 0, 0, WHITE); 

        EndDrawing();
    }

    // De-Initialization
    delete C;
    CloseWindow();        // Close window and OpenGL context

    return 0;
}