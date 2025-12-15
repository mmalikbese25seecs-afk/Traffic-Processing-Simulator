#pragma once
#include <raylib.h>
#include <cmath>
#include <queue>

#include "Utils.hpp"
#include "Debug.hpp"
#include "WindowConfig.hpp"

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
    float speed = 0.0f;

    // Turn
    float startAngle = 0.0f;
    float targetAngle = 0.0f;
};

struct Car
{
    Vector2 position = {0.0f, 0.0f};
    Vector2 size = {30.0f, 50.0f};
    float rotationAngle = 0.0f; // authoritative rotation
    Color color = WHITE;

    CarAction action{};
    std::queue<CarAction> actionQueue;
};

void UpdateCar(Car &car);
void DrawCar(const Car &car);

void QueueMove(Car &car, float speed, float duration);
void QueueTurn(Car &car, float angle, float duration);
