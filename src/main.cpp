#include <cmath>
#include <raylib.h>

#include "Car.hpp"
#include "TrafficLight.hpp"

#include "WindowConfig.hpp"

int main()
{
    TrafficLight trafficLight;
    trafficLight.position = {350.0f, 200.0f};
    trafficLight.size = {100.0f, 100.0f};

    Car myCar;
    myCar.position = WINDOW_CENTER;
    myCar.color = BLUE;

    // Initialize the window
    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_TITLE);
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // debug
        DrawFPS(10, 10);
        // debug end

        // update
        // UpdateTrafficLight(trafficLight);
        UpdateCar(myCar);
        // update end

        // draw
        // DrawTrafficLight(trafficLight);
        DrawCar(myCar);
        // draw end

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
