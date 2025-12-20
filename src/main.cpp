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

    initGameState(gameState);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // debug
        DrawFPS(10, 10);
        // debug end

        // update
        updateGameState(gameState);
        // update end

        // draw
        drawMainScreen(gameState);
        // draw end

        __ProcessDebugDraws();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
