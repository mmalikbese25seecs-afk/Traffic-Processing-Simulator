#include <cmath>
#include <raylib.h>

#include "Debug.hpp"
#include "WindowConfig.hpp"

#include "MainScreen.hpp"

int main()
{
    layouts::GameState gameState;

    // Initialize the window
    InitWindow(CFG::WINDOW_SIZE.x, CFG::WINDOW_SIZE.y, CFG::WINDOW_TITLE);
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // debug
        DrawFPS(10, 10);
        // debug end

        // update
        layouts::updateGameState(gameState);
        // update end

        // draw
        layouts::drawMainScreen(gameState);
        // draw end

        utils::__ProcessDebugDraws();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
