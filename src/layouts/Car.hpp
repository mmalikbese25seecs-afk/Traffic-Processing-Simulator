#pragma once
#include <raylib.h>
#include <cstdint>

struct GameState;

constexpr bool DEBUG_CAR_DETECTION_ARC = false;
constexpr bool DEBUG_CAR_DETECTION_OTHER_CARS = false;
constexpr bool DEBUG_CAR_STATE = true;

constexpr float CAR_WIDTH = 40.f;
constexpr float CAR_HEIGHT = 20.f;
constexpr float CAR_SPEED = 100.f;
// distance from car to other car to stop
constexpr float CAR_DETECTION_RADIUS = 50.f;
constexpr float CAR_DETECTION_ANGLE_DEG = 45.f;

enum class CarState
{
    MOVING,
    WAITING,
};

struct Car
{
    uint16_t id;
    Vector2 position;
    Vector2 size;
    Vector2 desiredVelocity; // where the car wants to go
    CarState state;
    Color color = BLUE;
    float rotation = 0.f;

    Vector2 _velocity; // current velocity
};

void UpdateCar(Car &car, GameState &state);
void DrawCar(const Car &car);
