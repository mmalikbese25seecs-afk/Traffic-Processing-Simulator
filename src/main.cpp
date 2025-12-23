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
    LoadConfigFromFile(gameState.rootConfigNode, "config.txt");

    // print config "speed"
    if (auto cfg = TryReadConfig(gameState.rootConfigNode, "speed"))
    {
        if (auto f = std::get_if<int>(&cfg->value))
            std::cout << "Config 'speed' = " << *f << "\n";
    }

    // print config "debug_mode"
    if (auto cfg = TryReadConfig(gameState.rootConfigNode, "debug_car_detection_other_cars"))
    {
        if (auto b = std::get_if<bool>(&cfg->value))
            std::cout << "Config 'debug_car_detection_other_cars' = " << (*b ? "true" : "false") << "\n";
    }
    GetConfigBool(gameState.rootConfigNode, "debug_car_detection_other_cars", false);

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
                // Apply all pending edits when closing UI
                ApplyPendingEdits(configUI);

                // Optional: save automatically
                SaveConfigToFile(gameState.rootConfigNode, "config.txt");
            }
        }

        if (IsKeyPressed(KEY_SPACE))
            std::cout << "TEST\n";

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

        __ProcessDebugDraws();

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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
