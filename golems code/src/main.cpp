#include <raylib.h>
#include <algorithm>
#include <iostream> // headers
#include <map>
#include <ctime>

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
    
    constexpr int screenWidth = 2000; // window dimensions
    constexpr int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Golems"); // initialize window
    SetTargetFPS(21);
    


    Image title = LoadImage("assets/golems game art.jpg"); // load the title image
    ImageResize(&title, titleWidth, titleHeight);
    Texture2D titleTexture = LoadTextureFromImage(title);   // convert image to texture
    UnloadImage(title); 

    enum GolemType // list of golem types
    {
        DEFAULT_GOLEM,
        RED_GOLEM,
        BLUE_GOLEM,
        RUBBLE,
        GOLEM_TYPE_COUNT
    };
    const int golemWidth = 200; // golem image dimensions
    const int golemHeight = 250;
    const char* golemPaths[GOLEM_TYPE_COUNT] = { // paths to golem images
        "assets/golem.png",
        "assets/red.png",
        "assets/blue.png",
        "assets/rubble.png"
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


    bool gameloop = false;// init game conditions
    bool help = false;
    bool wincondition = false;
    double wintime = 0.0;
    bool losecondition = false;
    double losetime = 0.0;
    int tries = 0;
    string guessHistory[10] = {};
    GolemType resultHistory[10][4] = {};
    
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
                int playY = titleY + titleHeight + 100; // place play button below title
                Rectangle playButton = {(float)playX, (float)playY, (float)buttonWidth, (float)buttonHeight};

                int helpX = (screenWidth - buttonWidth) / 2;
                int helpY = titleY + titleHeight + 200; // place help button below title
                Rectangle helpButton = {(float)helpX, (float)helpY, (float)buttonWidth, (float)buttonHeight};

                if (!help)
                {
                    bool playhovering = CheckCollisionPointRec(GetMousePosition(), playButton); // check if hovering over play button
                    DrawRectangleRec(playButton, playhovering ? buttonHoverColor : buttonColor);
                    DrawRectangleLinesEx(playButton, 3.0f, BLACK);

                    const char* playLabel = "PLAY"; // play button
                    int playSize = 40;
                    int playWidth = MeasureText(playLabel, playSize);
                    int playTextX = playX + (buttonWidth - playWidth) / 2;
                    int playTextY = playY + (buttonHeight - playSize) / 2;
                    DrawText(playLabel, playTextX, playTextY, playSize, WHITE);

                    bool helphovering = CheckCollisionPointRec(GetMousePosition(), helpButton); // check if hovering over help button
                    DrawRectangleRec(helpButton, helphovering ? buttonHoverColor : buttonColor);
                    DrawRectangleLinesEx(helpButton, 3.0f, BLACK);

                    const char* helpLabel = "HELP"; // help button
                    int helpSize = 40;
                    int helpWidth = MeasureText(helpLabel, helpSize);
                    int helpTextX = helpX + (buttonWidth - helpWidth) / 2;
                    int helpTextY = helpY + (buttonHeight - helpSize) / 2;
                    DrawText(helpLabel, helpTextX, helpTextY, helpSize, WHITE);

                    if (helphovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))    // help button press to show rules
                    {
                        help = true;
                    }

                    if (playhovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))    // play button press to game start
                    {
                        WaitTime(0.2f); // delay start
                        gameloop = true; // reset game conditions
                        wincondition = false;
                        wintime = 0.0;
                        losecondition = false;
                        losetime = 0.0;
                        tries = 0;
                        enteredSpell = "";
                        check_spell = "";
                        for (int i = 0; i < 4; i++)
                        {
                            golemSlots[i] = DEFAULT_GOLEM;
                        }
                        for (int i = 0; i < 10; i++)
                        {
                            guessHistory[i] = "";
                            for (int j = 0; j < 4; j++)
                            {
                                resultHistory[i][j] = DEFAULT_GOLEM;
                            }
                        }
                        currentSpell = secretspell();   // get secret spell
                    }
                }
                else
                {
                    Rectangle helpTextBox = {(float)(screenWidth / 2 - 650), (float)(titleY + titleHeight), 1300.0f, 230.0f};
                    DrawRectangleRec(helpTextBox, buttonHoverColor);
                    DrawRectangleLinesEx(helpTextBox, 3.0f, BLACK);

                    const char* helpLines[] = {
                        "The golems of Ankh Morpork are rising up under Adora after years of unfair labor.",
                        "The City Watch must stop them by guessing a 4-element control spell.",
                        "Each element is one of: Fire, Water, Air, Earth. Duplicates are allowed.",
                        "After each guess, golem eyes provide clues:",
                        "RED = correct element in the correct location.",
                        "BLUE = correct element in the wrong location.",
                        "Example: Secret FFAW, Guess FEWA -> RBB. Guess FAFW -> RRBB.",
                        "You only get 10 guesses to disable the golems."
                    };
                    int lineCount = sizeof(helpLines) / sizeof(helpLines[0]);
                    int helpTextSize = 29;
                    int lineHeight = 28;
                    int textStartY = (int)helpTextBox.y + ((int)helpTextBox.height - (lineCount * lineHeight)) / 2;
                    for (int i = 0; i < lineCount; i++)
                    {
                        int lineWidth = MeasureText(helpLines[i], helpTextSize);
                        int lineX = (int)helpTextBox.x + ((int)helpTextBox.width - lineWidth) / 2;
                        int lineY = textStartY + i * lineHeight;
                        DrawText(helpLines[i], lineX, lineY, helpTextSize, BLACK);
                    }

                    int exitY = helpY + 35;
                    Rectangle exitButton = {(float)helpX, (float)exitY, (float)buttonWidth, (float)buttonHeight};
                    bool exithovering = CheckCollisionPointRec(GetMousePosition(), exitButton); // help button replaced by exit button
                    DrawRectangleRec(exitButton, exithovering ? buttonHoverColor : buttonColor);
                    DrawRectangleLinesEx(exitButton, 3.0f, BLACK);

                    const char* exitLabel = "EXIT";
                    int exitSize = 40;
                    int exitWidth = MeasureText(exitLabel, exitSize);
                    int exitTextX = helpX + (buttonWidth - exitWidth) / 2;
                    int exitTextY = exitY + (buttonHeight - exitSize) / 2;
                    DrawText(exitLabel, exitTextX, exitTextY, exitSize, WHITE);

                    if (exithovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // return home
                    {
                        help = false;
                    }
                }
            }

            if (gameloop) // start 
            {
                constexpr int spacing = 20;
                int totalWidth = (golemTextures[DEFAULT_GOLEM].width * 4) + (spacing * (4 - 1));    // golem position
                int startX = (screenWidth - totalWidth) / 2;
                int startY = screenHeight - golemTextures[DEFAULT_GOLEM].height - 500;

                if (wincondition && GetTime() >= wintime)
                {
                    wincondition = false;
                    gameloop = false;
                }

                if (losecondition && GetTime() >= losetime)
                {
                    losecondition = false;
                    gameloop = false;
                }

                if (!wincondition && !losecondition && enteredSpell.size() < 4)    // enter guess if game is on
                {
                    if (IsKeyPressed(KEY_F)) enteredSpell.push_back('F');
                    else if (IsKeyPressed(KEY_W)) enteredSpell.push_back('W');
                    else if (IsKeyPressed(KEY_A)) enteredSpell.push_back('A');
                    else if (IsKeyPressed(KEY_E)) enteredSpell.push_back('E');
                }

                if (!wincondition && !losecondition && IsKeyPressed(KEY_BACKSPACE) && !enteredSpell.empty())   // backspace guess
                {
                    enteredSpell.pop_back();
                }

                for (int i = 0; i < 4; i++) // draw golems
                {
                    Texture2D currentTexture = golemTextures[golemSlots[i]];
                    int x = startX + i * (currentTexture.width + spacing);
                    DrawTexture(currentTexture, x, startY, WHITE);
                }

                const int miniScale = 4; // draw guesses and results
                int miniWidth = golemTextures[DEFAULT_GOLEM].width / miniScale;
                int miniHeight = golemTextures[DEFAULT_GOLEM].height / miniScale;
                int miniSpacing = 8;
                int historyCellWidth = (miniWidth * 4) + (miniSpacing * 3) + 100;
                int historyCellHeight = miniHeight + 26;
                int historyStartX = (screenWidth - (historyCellWidth * 5 + 40 * 3)) / 2;
                int historyStartY = startY + golemTextures[DEFAULT_GOLEM].height + 30;

                for (int i = 0; i < tries && i < 10 && !losecondition; i++)
                {
                    int column = i % 5;
                    int row = i / 5;
                    int cellX = historyStartX + column * (historyCellWidth + 40);
                    int cellY = historyStartY + row * (historyCellHeight + 18);

                    int golemSpan = (miniWidth * 4) + (miniSpacing * 3);
                    int miniStart = cellX;
                    int miniCenterX = miniStart + golemSpan / 2;

                    int textSize = 20;
                    int textWidth = MeasureText(guessHistory[i].c_str(), textSize);
                    int textX = miniCenterX - textWidth / 2;
                    DrawText(guessHistory[i].c_str(), textX, cellY, textSize, BLACK);

                    int miniY = cellY + 24;
                    for (int j = 0; j < 4; j++)
                    {
                        int miniX = miniStart + j * (miniWidth + miniSpacing);
                        Texture2D miniTexture = golemTextures[resultHistory[i][j]];
                        Rectangle src = {0.0f, 0.0f, (float)miniTexture.width, (float)miniTexture.height};
                        Rectangle dst = {(float)miniX, (float)miniY, (float)miniWidth, (float)miniHeight};
                        DrawTexturePro(miniTexture, src, dst, {0.0f, 0.0f}, 0.0f, WHITE);
                    }
                }

                // if (IsKeyPressed(KEY_R))
                // {
                //     currentSpell = secretspell(); // reroll 
                // }

                if (!losecondition)
                {
                    // int spellSize = 20; // current spell position
                    // int textWidth = MeasureText(currentSpell.c_str(), spellSize);
                    // int spellX = (screenWidth - textWidth) / 2;
                    // int spellY = startY - spellSize - 20;
                    // int drawX = spellX;
                    // for (char c : currentSpell)
                    // {
                    //     char letter[2] = {c, '\0'};
                    //     DrawText(letter, drawX, spellY, spellSize, elementColor(c));
                    //     drawX += MeasureText(letter, spellSize);
                    // }
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
                }

                if (!wincondition && !losecondition && tries < 10 && IsKeyPressed(KEY_ENTER) && enteredSpell.size() == 4){   // enter guess logic
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
                            resultHistory[tries][i] = DEFAULT_GOLEM;
                        }

                        for (int i = 0; i < red && i < 4; i++)
                        {
                            golemSlots[i] = RED_GOLEM;
                            resultHistory[tries][i] = RED_GOLEM;
                        }

                        for (int i = red; i < red + blue && i < 4; i++)
                        {
                            golemSlots[i] = BLUE_GOLEM;
                            resultHistory[tries][i] = BLUE_GOLEM;
                        }

                        guessHistory[tries] = check_spell;

                        if (red == 4) { // win condition
                            for (int i = 0; i < 4; i++)
                            {
                                golemSlots[i] = RUBBLE;
                            }
                            tries++;
                            wincondition = true;
                            wintime = GetTime() + 2.0; // win timer
                            continue;
                        }
                        tries++;
                        if (tries >=10)   // lose condition (only after non-winning guess)
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                golemSlots[i] = DEFAULT_GOLEM;
                            }
                            enteredSpell = "";
                            check_spell = "";
                            for (int i = 0; i < 10; i++)
                            {
                                guessHistory[i] = "";
                                for (int j = 0; j < 4; j++)
                                {
                                    resultHistory[i][j] = DEFAULT_GOLEM;
                                }
                            }
                            losecondition = true;
                            losetime = GetTime() + 2.0;
                            continue;
                        }
                }

                if (losecondition) // lose ending
                {
                    const char* loseLabel = "YOU LOSE";
                    int loseSize = 80;
                    int loseWidth = MeasureText(loseLabel, loseSize);
                    int loseX = (screenWidth - loseWidth) / 2;
                    int loseY = (screenHeight - loseSize) / 2;
                    DrawText(loseLabel, loseX, loseY, loseSize, BLACK);
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