#include <raylib.h>
#include <iostream>
using namespace std;

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Golems");
    SetTargetFPS(8);
    
    while (!WindowShouldClose())
    {
        
        BeginDrawing();
            ClearBackground(darkGreen);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}