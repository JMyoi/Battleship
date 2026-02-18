#include "raylib.h"
#include <iostream>
#include "Board.hpp"
using namespace std;


int main(void)
{
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Battleship");
    SetTargetFPS(60);
    Board b;
    
    while (!WindowShouldClose())
    {
        //1, Event handling
        //2. update

        //3. Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);
            b.Draw(Vector2{10,10}, Vector2{500,500});


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        

    return 0;
}