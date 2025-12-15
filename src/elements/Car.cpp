#include "Car.hpp"

void UpdateCar(Car &car)
{
    // if no actions, return
    if (car.action.type == CarActionType::None && car.actionQueue.empty())
        return;

    // pull next action if none active
    if (car.action.type == CarActionType::None && !car.actionQueue.empty())
    {
        car.action = car.actionQueue.front();
        car.actionQueue.pop();
        car.action.elapsed = 0.0f;

        if (car.action.type == CarActionType::Turn)
        {
            // set initial angle for turn
            car.action.startAngle = car.rotationAngle;
        }
    }

    float deltaTime = GetFrameTime();
    car.action.elapsed += deltaTime;

    // calculate forward vector
    float radians = ConvertDegreesToRadians(car.rotationAngle);
    Vector2 forward = {cos(radians), sin(radians)};

    DebugDrawVector(car.position, forward, GREEN, 100);

    switch (car.action.type)
    {
    case CarActionType::Move:
    {
        // calculate forward vector
        float radians = ConvertDegreesToRadians(car.rotationAngle);
        Vector2 forward = {cos(radians), sin(radians)}; // 0 deg = up

        // position += forward * speed * deltaTime
        car.position.x += forward.x * car.action.speed * deltaTime;
        car.position.y += forward.y * car.action.speed * deltaTime;

        DebugDrawVector(car.position, forward, GREEN, 100);

        // time up for this move action
        if (car.action.elapsed >= car.action.duration)
        {
            car.action.type = CarActionType::None;
        }
        break;
    }

    case CarActionType::Turn:
    {
        float elapsedTime = car.action.elapsed / car.action.duration;
        Clamp(elapsedTime, 0.0f, 1.0f);

        // time up for this turn action
        if (elapsedTime >= 1.0f)
        {
            car.rotationAngle = car.action.targetAngle;
            car.action.type = CarActionType::None;
        }
        else
        {
            // lerp; lerp(a, b, t) = a * (1 - t) + b * t
            car.rotationAngle = car.action.startAngle * (1 - elapsedTime) + car.action.targetAngle * elapsedTime;
        }

        // wrap angle between 0-360
        WrapAngle360(car.rotationAngle);
        break;
    }

    case CarActionType::None:
        break;
    }
}

void DrawCar(const Car &car)
{
    DrawRectanglePro(
        {car.position.x, car.position.y, car.size.x, car.size.y},
        {car.size.x * 0.5f, car.size.y * 0.5f},
        car.rotationAngle,
        car.color);
}

void QueueMove(Car &car, float speed, float duration)
{
    CarAction action;
    action.type = CarActionType::Move;
    action.duration = duration;
    action.speed = speed;

    car.actionQueue.push(action);
}

void QueueTurn(Car &car, float angle, float duration)
{
    CarAction action;
    action.type = CarActionType::Turn;
    action.duration = duration;
    action.targetAngle = angle;

    car.actionQueue.push(action);
}
