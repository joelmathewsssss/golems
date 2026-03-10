#include <raylib.h>
#include <iostream>
using namespace std;

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 1000;
    constexpr int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Golems");
    SetTargetFPS(8);
    

    Image image = LoadImage("assets/golems game art.jpg"); // Load the image
    Texture2D texture = LoadTextureFromImage(image);   // Convert image to texture
    UnloadImage(image); 
    
    while (!WindowShouldClose())
    {
        
        BeginDrawing();
            DrawTexture(texture, 100, 100, WHITE); // Draw at (100, 100)
            ClearBackground(darkGreen);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}