#include <raylib.h>
#include <algorithm>
#include <iostream> // headers
#include <map>


using namespace std;


int main() 
{   
    const Color sand = {194, 178, 128, 255}; // custom bg color
    const Color buttonColor = {170, 150, 109, 255};
    const Color buttonHoverColor = {194, 178, 128, 255};
    
    constexpr int titleWidth = 800; // title image dimensions
    constexpr int titleHeight = 300;

    constexpr int buttonWidth = 260; // button dimensions
    constexpr int buttonHeight = 80;
    
    constexpr int screenWidth = 1000; // window dimensions
    constexpr int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Golems"); // initialize window
    SetTargetFPS(8);
    


    Image title = LoadImage("assets/golems game art.jpg"); // load the title image
    ImageResize(&title, titleWidth, titleHeight);
    Texture2D titleTexture = LoadTextureFromImage(title);   // convert image to texture
    UnloadImage(title); 

    enum GolemType // list of golem types
    {
        DEFAULT_GOLEM,
        RED_GOLEM,
        BLUE_GOLEM,
        GOLEM_TYPE_COUNT
    };
    const int golemWidth = 200; // golem image dimensions
    const int golemHeight = 250;
    const char* golemPaths[GOLEM_TYPE_COUNT] = { // paths to golem images
        "assets/golem.png",
        "assets/red.png",
        "assets/blue.png"
    };
    Texture2D golemTextures[GOLEM_TYPE_COUNT] = {}; // array to hold golem textures
    for (int i = 0; i < GOLEM_TYPE_COUNT; i++)  // draw list of golems
    {
        Image golemImage = LoadImage(golemPaths[i]);
        ImageResize(&golemImage, golemWidth, golemHeight);
        golemTextures[i] = LoadTextureFromImage(golemImage);
        UnloadImage(golemImage);
    }
    GolemType golemSlots[4] = {DEFAULT_GOLEM, DEFAULT_GOLEM, DEFAULT_GOLEM, DEFAULT_GOLEM}; // default declaration of golem slots

    auto secretspell = []() -> string {     // function to generate random spell
        const char options[4] = {'F', 'W', 'E', 'A'};
        string spell;
        for (int i = 0; i < 4; i++)
        {
            int randomint = GetRandomValue(0, 3);
            spell.push_back(options[randomint]);
        }
        return spell;
    };

    auto elementColor = [](char c) -> Color {   // function to color element letters
        switch (c)
        {
            case 'F': return {255, 98, 0, 255};   // fire
            case 'W': return {0, 8, 255, 255};  // water
            case 'A': return {130, 237, 242, 255}; // air
            case 'E': return {31, 153, 37, 255};   // earth
            default: return BLACK;
        }
    };

    string currentSpell = "";   // init spell variables
    string enteredSpell = "";
    string check_spell = "";


    bool gameloop = false;
    double gameStartTime = 0.0; // variable to track when the game starts
    
    while (!WindowShouldClose())
    {
        
        BeginDrawing();
        
            ClearBackground(sand);

            if (!gameloop)  // while not in game (title screen)
            {
                int titleX = (screenWidth - titleWidth) / 2; // center the title
                int titleY = (screenHeight - titleHeight) / 2 - 200;
                DrawTexture(titleTexture, titleX, titleY, WHITE);

                int playX = (screenWidth - buttonWidth) / 2;
                int playY = titleY + titleHeight + 100; // place button below title
                Rectangle playButton = {(float)playX, (float)playY, (float)buttonWidth, (float)buttonHeight};

                bool playhovering = CheckCollisionPointRec(GetMousePosition(), playButton); // check if hovering over play button
                DrawRectangleRec(playButton, playhovering ? buttonHoverColor : buttonColor);
                DrawRectangleLinesEx(playButton, 3.0f, BLACK);

                const char* playLabel = "PLAY"; // play button
                int playSize = 40;
                int playWidth = MeasureText(playLabel, playSize);
                int playTextX = playX + (buttonWidth - playWidth) / 2;
                int playTextY = playY + (buttonHeight - playSize) / 2;
                DrawText(playLabel, playTextX, playTextY, playSize, WHITE);

                if (playhovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))    // play button press to game start
                {
                    gameloop = true;
                    gameStartTime = GetTime();
                    currentSpell = secretspell();   // get secret spell
                }
            }

            if (gameloop && (GetTime() - gameStartTime) >= 0.2) // start after delay
            {
                constexpr int spacing = 20;
                int totalWidth = (golemTextures[DEFAULT_GOLEM].width * 4) + (spacing * (4 - 1));    // golem position
                int startX = (screenWidth - totalWidth) / 2;
                int startY = screenHeight - golemTextures[DEFAULT_GOLEM].height - 500;

                if (enteredSpell.size() < 4)    // enter guess
                {
                    if (IsKeyPressed(KEY_F)) enteredSpell.push_back('F');
                    else if (IsKeyPressed(KEY_W)) enteredSpell.push_back('W');
                    else if (IsKeyPressed(KEY_A)) enteredSpell.push_back('A');
                    else if (IsKeyPressed(KEY_E)) enteredSpell.push_back('E');
                }

                if (IsKeyPressed(KEY_BACKSPACE) && !enteredSpell.empty())   // backspace guess
                {
                    enteredSpell.pop_back();
                }

                for (int i = 0; i < 4; i++) // draw golems
                {
                    Texture2D currentTexture = golemTextures[golemSlots[i]];
                    int x = startX + i * (currentTexture.width + spacing);
                    DrawTexture(currentTexture, x, startY, WHITE);
                }

                // if (IsKeyPressed(KEY_R))
                // {
                //     currentSpell = secretspell(); // reroll 
                // }

                int spellSize = 20; // current spell position
                int textWidth = MeasureText(currentSpell.c_str(), spellSize);
                int spellX = (screenWidth - textWidth) / 2;
                int spellY = startY - spellSize - 20;
                int drawX = spellX;
                for (char c : currentSpell)
                {
                    char letter[2] = {c, '\0'};
                    DrawText(letter, drawX, spellY, spellSize, elementColor(c));
                    drawX += MeasureText(letter, spellSize);
                }
                int slotY = startY + golemTextures[DEFAULT_GOLEM].height + 400;
                for (int i = 0; i < 4; i++) // draw guess slots
                {
                    int slotX = startX + i * (golemTextures[DEFAULT_GOLEM].width + spacing + 50);
                    int lineWidth = 70;
                    int lineY = slotY + 50;
                    DrawLine(slotX, lineY, slotX + lineWidth, lineY, BLACK);

                    if (i < (int)enteredSpell.size())   // draw current guess letters
                    {
                        char letter[2] = {enteredSpell[i], '\0'};
                        int letterSize = 34;
                        int letterWidth = MeasureText(letter, letterSize);
                        int letterX = slotX + (lineWidth - letterWidth) / 2;
                        int letterY = lineY - letterSize - 6;
                        DrawText(letter, letterX, letterY, letterSize, elementColor(enteredSpell[i]));
                    }
                }
                if (IsKeyPressed(KEY_ENTER) && enteredSpell.size() == 4){   // enter guess logic
                    check_spell = enteredSpell;
                    enteredSpell = "";
                    int red = 0;
                    int blue = 0;
                    map<char, int> spellcount;
                    map<char, int> guesscount;

                    for (int i = 0; i < 4; i++){    // check how many match (red)
                        if (check_spell[i] == currentSpell[i]){
                            red++; 
                        }
                        else
                        {
                            spellcount[currentSpell[i]]++;  // wtv doesnt match
                            guesscount[check_spell[i]]++;
                        }
                    }
                    
                    for (const auto& pair : spellcount){    // check if theres matches and take lowest frequency (blue)
                        char sletter = pair.first;
                        int scount = pair.second;
                        if (guesscount.count(sletter) > 0){
                            blue += min(scount, guesscount[sletter]);
                        }
                    }

                    for (int i = 0; i < 4; i++)  // draw golems based on guess results
                    {
                        golemSlots[i] = DEFAULT_GOLEM;
                    }

                    for (int i = 0; i < red && i < 4; i++)
                    {
                        golemSlots[i] = RED_GOLEM;
                    }

                    for (int i = red; i < red + blue && i < 4; i++)
                    {
                        golemSlots[i] = BLUE_GOLEM;
                    }
                }


            }            



        

            EndDrawing();   // end of drawing loop
    }
    
        UnloadTexture(titleTexture);
        for (int i = 0; i < GOLEM_TYPE_COUNT; i++)
        {
            UnloadTexture(golemTextures[i]);
        }
    CloseWindow();

    return 0;
}