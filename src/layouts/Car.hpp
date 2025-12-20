#pragma once
#include <raylib.h>

#include "MainScreen.hpp"

struct GameState;

struct Car
{
    Vector2 position;
    Vector2 desiredVelocity; // where the car wants to go
    Color color = BLUE;
    Vector2 velocity; // current velocity
};

void UpdateCar(Car &car, const GameState &state);
void DrawCar(const Car &car);

void SetCarVelocity(Car &car, Vector2 newVelocity);
Vector2 GetCarVelocity(const Car &car);
