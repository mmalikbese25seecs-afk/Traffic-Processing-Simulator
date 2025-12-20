#pragma once
#include <raylib.h>

struct GameState;

constexpr float CAR_WIDTH = 20.f;
constexpr float CAR_HEIGHT = 40.f;
constexpr float CAR_SPEED = 100.f;
// distance from car to other car to stop
constexpr float CAR_REAR_END_DISTANCE = 50.f;

struct Car
{
    Vector2 position;
    Vector2 size;
    Vector2 desiredVelocity; // where the car wants to go
    Color color = BLUE;

    Vector2 _velocity; // current velocity
};

void UpdateCar(Car &car, const GameState &state);
void DrawCar(const Car &car);

void SetCarVelocity(Car &car, Vector2 newVelocity);
Vector2 GetCarVelocity(const Car &car);

void StopCar(Car &car);
void ResumeCar(Car &car);
