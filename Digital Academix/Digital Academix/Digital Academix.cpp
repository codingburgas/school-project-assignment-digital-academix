#include "raylib.h"
#include <string> 
#include <fstream>
#include <iostream>
#include "functions.h"


int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Digital Academix");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);
        DrawMainMenu();
        
    }
}




