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

    // car driver
    QueueMove(myCar, 100.0f, 2.0f); // move right for 2 seconds
    QueueTurn(myCar, 90.0f, 1.0f);  // turn down over 1 second
    QueueMove(myCar, 100.0f, 2.0f); // move down for 2 seconds
    QueueTurn(myCar, 180.0f, 1.0f); // turn left over 1 second
    QueueMove(myCar, 100.0f, 2.0f); // move left for 2 seconds
    QueueTurn(myCar, 720.0f, 1.0f); // turn up over 1 second
    QueueMove(myCar, 100.0f, 2.0f); // move up for 2 seconds

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
