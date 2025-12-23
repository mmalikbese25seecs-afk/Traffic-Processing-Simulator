#pragma once
#include <raylib.h>
#include <cstdint>

struct GameState;


constexpr float CAR_WIDTH = 40.f;
constexpr float CAR_HEIGHT = 20.f;
constexpr float CAR_SPEED = 100.f;

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
void DrawCar(const Car &car, const GameState& state);
