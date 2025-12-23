#include <cmath>
#include <raylib.h>

#include "Debug.hpp"
#include "WindowConfig.hpp"
#include "MainScreen.hpp"

#include "ConfigUI.hpp"
#include "ConfigIO.hpp"

#include "config.hpp"

int main()
{
    GameState gameState;

    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_TITLE);
    SetTargetFPS(60);

    InitGameState(gameState);

    gameState.rootConfigNode = BuildConfigTree();
    ConfigUIState configUI;
    bool showConfig = false;

    SetExitKey(KEY_NULL);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_C)) // toggle config UI
        {
            showConfig = !showConfig;

            if (!showConfig)
            {
                // UI just closed
                // Apply any pending edit buffer
                if (configUI.editing && configUI.selected)
                {
                    ApplyTextEdit(*configUI.selected, configUI.editBuffer);
                    configUI.editing = false;
                    configUI.selected = nullptr;
                }

                // Optional: save config automatically on close
                SaveConfigToFile(gameState.rootConfigNode, "config.txt");
            }
        }

        if (showConfig)
        {
            if (IsKeyPressed(KEY_S))
                SaveConfigToFile(gameState.rootConfigNode, "config.txt");

            if (IsKeyPressed(KEY_L))
                LoadConfigFromFile(gameState.rootConfigNode, "config.txt");
        }

        if (IsKeyPressed(KEY_R))
        {
            RestartGameState(gameState);
        }
        else if (IsKeyPressed(KEY_P))
        {
            if (gameState.paused)
                ResumeGameState(gameState);
            else
                PauseGameState(gameState);
        }

        if (!showConfig)
        {
            UpdateGameState(gameState);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawMainScreen(gameState);

        if (showConfig)
        {
            DrawRectangle(
                0,
                0,
                GetScreenWidth(),
                GetScreenHeight(),
                Fade(BLACK, 0.6f));

            std::string help_text = "C = Close   S = Save   L = Load";
            int textWidth = MeasureText(help_text.c_str(), 16);
            DrawText(help_text.c_str(), WINDOW_SIZE.x - textWidth - 16, 10, 16, RAYWHITE);

            DrawConfigUI(gameState.rootConfigNode, configUI, 40.0f, WINDOW_SIZE.y - 64.f, WINDOW_SIZE.x / 2.f);
        }

        DrawFPS(10, 10);
        __ProcessDebugDraws();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
