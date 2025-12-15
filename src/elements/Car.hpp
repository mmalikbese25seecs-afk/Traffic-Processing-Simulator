#pragma once
#include <raylib.h>

struct Car
{
    Vector2 position;
    Vector2 size = {50.0f, 30.0f};
    Vector2 velocity = {100.0f, 100.0f}; // pixels per second
    Color color;

    // turn
    float turnTime = 1.0f; // time to complete a 90 deg turn; in seconds
    bool isTurning = false;
    float turnStartAngle = 0.0f;
    float turnTargetAngle = 0.0f;
    float turnElapsed = 0.0f;
};

void UpdateCar(Car &car);
void DrawCar(const Car &car);

void TurnCar(Car &car, float angle);

// get rotation angle in degrees from velocity
float GetRotationAngle(const Car &car);
// set rotation angle in degrees to velocity
void SetRotationAngle(Car &car, float angle);

// private functions
void _turnCarUpdate(Car &car);
void _moveCarUpdate(Car &car);