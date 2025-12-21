#include <cmath>
#include <raylib.h>

#include "Debug.hpp"
#include "WindowConfig.hpp"

#include "MainScreen.hpp"

int main()
{
    GameState gameState;

    // Initialize the window
    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_TITLE);
    SetTargetFPS(60);

    InitGameState(gameState);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_R))
        {
            RestartGameState(gameState);
        }

        // update
        UpdateGameState(gameState);
        // update end

        // draw
        DrawMainScreen(gameState);
        // draw end

        // debug
        DrawFPS(10, 10);
        __ProcessDebugDraws();
        // debug end

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
