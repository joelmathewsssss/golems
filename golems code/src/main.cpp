#include <raylib.h>
#include <iostream>


using namespace std;

int main() 
{   
    const Color darkGreen = {20, 160, 133, 255}; // Custom dark green color
    
    constexpr int artWidth = 800;
    constexpr int artHeight = 400;
    
    constexpr int screenWidth = 1000;
    constexpr int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Golems");
    SetTargetFPS(8);
    

    Image image = LoadImage("assets/golems game art.jpg"); // Load the image
        ImageResize(&image, artWidth, artHeight);
    Texture2D texture = LoadTextureFromImage(image);   // Convert image to texture
    UnloadImage(image); 
    
    while (!WindowShouldClose())
    {
        
        BeginDrawing();
        
            ClearBackground(darkGreen);
            int artX = (screenWidth - artWidth) / 2;
            int artY = (screenHeight - artHeight) / 2;
            DrawTexture(texture, artX, artY, WHITE);
            EndDrawing();
    }
    
        UnloadTexture(texture);
    CloseWindow();

    return 0;
}