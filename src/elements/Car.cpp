#include "Car.hpp"

#include <cmath>

#include "Utils.hpp"
#include "WindowConfig.hpp"

void UpdateCar(Car &car)
{
    _turnCarUpdate(car);

    _moveCarUpdate(car);
}

void DrawCar(const Car &car)
{
    // Draw the car as a rectangle with rotation
    DrawRectanglePro(
        {car.position.x, car.position.y, car.size.x, car.size.y},
        {car.size.x / 2, car.size.y / 2}, // origin at center
        GetRotationAngle(car),
        car.color);
}

void TurnCar(Car &car, float angle)
{
    if (car.isTurning)
        return;

    car.isTurning = true;
    car.turnElapsed = 0.0f;
    car.turnStartAngle = GetRotationAngle(car);
    car.turnTargetAngle = GetRotationAngle(car) + angle;
}

float GetRotationAngle(const Car &car)
{
    return ConvertRadiansToDegrees(atan2(car.velocity.y, car.velocity.x));
}

void SetRotationAngle(Car &car, float angle)
{
    float radians = ConvertDegreesToRadians(angle);
    car.velocity.x = cos(radians) * sqrt(pow(car.velocity.x, 2) + pow(car.velocity.y, 2));
    car.velocity.y = sin(radians) * sqrt(pow(car.velocity.x, 2) + pow(car.velocity.y, 2));
}

void _turnCarUpdate(Car &car)
{
    // car turn logic
    if (car.isTurning)
    {
        car.turnElapsed += GetFrameTime();
        float turnProgress = car.turnElapsed / car.turnTime;

        if (turnProgress >= 1.0f)
        {
            SetRotationAngle(car, car.turnTargetAngle);
            car.isTurning = false;
        }
        else
        {
            float currentAngle = car.turnStartAngle + (car.turnTargetAngle - car.turnStartAngle) * turnProgress;
            SetRotationAngle(car, currentAngle);
        }
    }
}

void _moveCarUpdate(Car &car)
{
    // get forward vector (up for 0 degrees)
    float radians = ConvertDegreesToRadians(GetRotationAngle(car));
    Vector2 forward = {cos(radians), sin(radians)};

    // draw forward vector for debugging
    DebugDrawVector(car.position, forward, YELLOW, 50.0f);

    // move car forward
    car.position.x += forward.x * car.velocity.x * GetFrameTime();
    car.position.y += forward.y * car.velocity.y * GetFrameTime();

    // keep car within window bounds (assuming window size 800x600)
    // clang-format off
    if (car.position.x < 0)             car.position.x = WINDOW_SIZE.x;
    if (car.position.x > WINDOW_SIZE.x) car.position.x = 0;
    if (car.position.y < 0)             car.position.y = WINDOW_SIZE.y;
    if (car.position.y > WINDOW_SIZE.y) car.position.y = 0;
    // clang-format on
}
