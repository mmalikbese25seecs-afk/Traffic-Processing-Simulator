#pragma once
#include <raylib.h>

struct GameState;

constexpr bool DEBUG_CAR_DETECTION_ARC = false;
constexpr bool DEBUG_CAR_DETECTION_OTHER_CARS = false;

constexpr float CAR_WIDTH = 40.f;
constexpr float CAR_HEIGHT = 20.f;
constexpr float CAR_SPEED = 100.f;
// distance from car to other car to stop
constexpr float CAR_DETECTION_RADIUS = 50.f;
constexpr float CAR_DETECTION_ANGLE_DEG = 45.f;

struct Car
{
    Vector2 position;
    Vector2 size;
    Vector2 desiredVelocity; // where the car wants to go
    Color color = BLUE;
    float rotation = 0.f;

    Vector2 _velocity; // current velocity
};

void UpdateCar(Car &car, const GameState &state);
void DrawCar(const Car &car);

const Vector2 &NewFunction(const Car &car);

void SetCarVelocity(Car &car, Vector2 newVelocity);
Vector2 GetCarVelocity(const Car &car);

void StopCar(Car &car);
void ResumeCar(Car &car);
