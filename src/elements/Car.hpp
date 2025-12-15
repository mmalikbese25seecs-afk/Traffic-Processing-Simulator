#pragma once
#include <raylib.h>
#include <queue>

enum class CarActionType
{
    None,
    Move,
    Turn
};

struct CarAction
{
    CarActionType type = CarActionType::None;
    float duration = 0.0f;
    float elapsed = 0.0f;

    // Move
    Vector2 direction = {0.0f, 0.0f};
    float speed = 0.0f;

    // Turn
    float startAngle = 0.0f;
    float targetAngle = 0.0f;
};

struct Car
{
    Vector2 position = {0.0f, 0.0f};
    Vector2 size = {50.0f, 30.0f};
    float rotationAngle = 0.0f; // authoritative rotation
    Color color = WHITE;

    CarAction action{};
    std::queue<CarAction> actionQueue;
};

void UpdateCar(Car &car);
void DrawCar(const Car &car);

void QueueMove(Car &car, const Vector2 &direction, float speed, float duration);
void QueueTurn(Car &car, float angle, float duration);
