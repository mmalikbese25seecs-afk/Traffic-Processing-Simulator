#include <cmath>
#include <raylib.h>

#include "Debug.hpp"
#include "WindowConfig.hpp"
#include "MainScreen.hpp"

#include "ConfigUI.hpp"
#include "ConfigIO.hpp"

// forward declaration
static Node BuildConfigTree()
{
    Node root;
    root.title = "CONFIG,";

    Node main_config;
    main_config.title = "Main Configuration";

    Node car;
    car.title = "Car";

    Node movement;
    movement.title = "Movement";
    movement.children.push_back(
        Node{"Speed", ConfigValue{"speed", ValueType::Int, 100, 0, 300}, {}, false});
    car.children.push_back(movement);

    Node detection;
    detection.title = "Detection";
    detection.children.push_back(
        Node{"Radius", ConfigValue{"radius", ValueType::Int, 50, 0, 200}, {}, false});
    detection.children.push_back(
        Node{"Angle(deg)", ConfigValue{"angle_deg", ValueType::Int, 45, 0, 180}, {}, false});
    car.children.push_back(detection);

    Node spawn;
    spawn.title = "Car Spawning";
    spawn.children.push_back(
        Node{"Start Offset", ConfigValue{"start_offset", ValueType::Int, -10, -100, 100}, {}, false});
    spawn.children.push_back(
        Node{"Spacing", ConfigValue{"spacing", ValueType::Int, 50, 0, 500}, {}, false});
    spawn.children.push_back(
        Node{"Main Screen Spawn Chance",
             ConfigValue{"main_screen_spawn_chance", ValueType::Float, 1.0f, 0.0f, 1.0f},
             {},
             false});

    Node tl;
    tl.title = "Traffic Light";
    tl.children.push_back(
        Node{"Wait Pos Offset", ConfigValue{"wait_pos_offset", ValueType::Int, 20, 0, 100}, {}, false});

    Node tl_det;
    tl_det.title = "Detection";
    tl_det.children.push_back(
        Node{"Car Detection Range", ConfigValue{"car_detection_range", ValueType::Int, 50, 0, 200}, {}, false});
    tl.children.push_back(tl_det);

    Node tl_timing;
    tl_timing.title = "Timing";
    tl_timing.children.push_back(
        Node{"Green Duration", ConfigValue{"green_duration", ValueType::Int, 10, 1, 60}, {}, false});
    tl_timing.children.push_back(
        Node{"Yellow Duration", ConfigValue{"yellow_duration", ValueType::Int, 3, 1, 10}, {}, false});
    tl_timing.children.push_back(
        Node{"All Red Duration",
             ConfigValue{"all_red_duration", ValueType::Float, 0.5f, 0.0f, 5.0f},
             {},
             false});
    tl.children.push_back(tl_timing);

    Node tl_adaptive;
    tl_adaptive.title = "Adaptive";
    tl_adaptive.children.push_back(
        Node{"Enabled", ConfigValue{"enabled", ValueType::Bool, true}, {}, false});
    tl_adaptive.children.push_back(
        Node{"Tick Rate", ConfigValue{"tick_rate", ValueType::Int, 1, 1, 10}, {}, false});
    tl_adaptive.children.push_back(
        Node{"Min Green Time", ConfigValue{"min_green_time", ValueType::Int, 5, 1, 20}, {}, false});
    tl_adaptive.children.push_back(
        Node{"Max Green Time", ConfigValue{"max_green_time", ValueType::Int, 20, 5, 60}, {}, false});
    tl.children.push_back(tl_adaptive);

    Node debug_options;
    debug_options.title = "Debug Options";

    debug_options.children.push_back(
        Node{"Enabled", ConfigValue{"debug_enabled", ValueType::Bool, false}, {}, false});

    Node debug_car;
    debug_car.title = "Car";
    debug_car.children.push_back(
        Node{"Detection Arc", ConfigValue{"debug_car_detection_arc", ValueType::Bool, false}, {}, false});
    debug_car.children.push_back(
        Node{"Detection Other Cars", ConfigValue{"debug_car_detection_other_cars", ValueType::Bool, false}, {}, false});
    debug_car.children.push_back(
        Node{"State", ConfigValue{"debug_car_state", ValueType::Bool, false}, {}, false});
    debug_options.children.push_back(debug_car);

    Node debug_tl;
    debug_tl.title = "Traffic Light";
    debug_tl.children.push_back(
        Node{"Car Can Pass", ConfigValue{"debug_tl_car_can_pass", ValueType::Bool, false}, {}, false});
    debug_tl.children.push_back(
        Node{"Car Passed", ConfigValue{"debug_tl_car_passed", ValueType::Bool, false}, {}, false});
    debug_tl.children.push_back(
        Node{"Draw Positions", ConfigValue{"debug_tl_draw_positions", ValueType::Bool, false}, {}, false});
    debug_options.children.push_back(debug_tl);

    main_config.children.push_back(car);
    main_config.children.push_back(spawn);
    main_config.children.push_back(tl);

    root.children.push_back(main_config);
    root.children.push_back(debug_options);

    return root;
}

int main()
{
    GameState gameState;

    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_TITLE);
    SetTargetFPS(60);

    InitGameState(gameState);

    Node configRoot = BuildConfigTree();
    ConfigUIState configUI;
    bool showConfig = false;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_C))
        {
            showConfig = !showConfig;
        }

        if (showConfig)
        {
            if (IsKeyPressed(KEY_S))
                SaveConfigToFile(configRoot, "config.txt");

            if (IsKeyPressed(KEY_L))
                LoadConfigFromFile(configRoot, "config.txt");
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

            DrawConfigUI(configRoot, configUI, 40.0f, WINDOW_SIZE.y - 64.f, WINDOW_SIZE.x / 2.f);
        }

        DrawFPS(10, 10);
        __ProcessDebugDraws();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
