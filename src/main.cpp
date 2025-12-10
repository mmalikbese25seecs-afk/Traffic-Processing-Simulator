#include "raylib.h"
#include <cmath>

int main()
{
    // Initialize the window
    InitWindow(800, 600, "Traffic Lights Simulator");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // show fps
        DrawFPS(10, 10);

        // get input
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            DrawRectangle(350, 250, 100, 100, GREEN);
        }
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            DrawRectangle(350, 250, 100, 100, RED);
        }
        else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            DrawRectangle(350, 250, 100, 100, BLUE);
        }
        else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            DrawRectangle(350, 250, 100, 100, YELLOW);
        }
        else
        {
            DrawRectangle(350, 250, 100, 100, GRAY);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
