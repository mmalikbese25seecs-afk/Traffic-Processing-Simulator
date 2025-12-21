#include "Car.hpp"

#include <iostream>
#include <cmath>

#include "MainScreen.hpp"
#include "Debug.hpp"
#include "VectorMath.hpp"

void m_updateVelocity(Car &car, const GameState &state)
{
    if (state.paused)
        return;

    car.position.x += car._velocity.x * state.deltaTime;
    car.position.y += car._velocity.y * state.deltaTime;
}

bool m_canCarMove(const Car &car, const GameState &state)
{
    for (const auto &otherCar : state.cars)
    {
        // skip self
        if (&otherCar == &car)
            continue;

        float distance = Vector2Distance(car.position, otherCar.position);
        if (distance < CAR_DETECTION_RADIUS)
        {
            // check if other car is in front of this car
            Vector2 toOther = {
                otherCar.position.x - car.position.x,
                otherCar.position.y - car.position.y //
            };
            Vector2Normalize(toOther);
            Vector2 desiredDir = car.desiredVelocity;
            Vector2Normalize(desiredDir);

            if (DEBUG_CAR_DETECTION_OTHER_CARS)
            {
                __DebugDrawVector(car.position, toOther, distance, 2, MAGENTA);
                __DebugDrawPoint(otherCar.position, 4.f, MAGENTA);
            }

            float dot = Vector2Dot(toOther, desiredDir);
            // other car is in front
            if (dot > cosf(AngleToRadians(45.f)))
                return false;
        }
    }

    // no car in front
    return true;
}

void UpdateCar(Car &car, const GameState &state)
{
    if (DEBUG_CAR_DETECTION_ARC)
    {
        if (Vector2IsZero(car.desiredVelocity))
            return;

        // cannot determine direction
        Vector2 forward = car.desiredVelocity;
        Vector2Normalize(forward);

        // compute forward angle in degrees (raylib space)
        float forwardAngleDeg = atan2f(forward.y, forward.x) * RAD2DEG;

        float startAngle = forwardAngleDeg - 45.0f;
        float endAngle = forwardAngleDeg + 45.0f;

        // debug: draw detection arc
        __DebugDrawCircleArc(car.position, CAR_DETECTION_RADIUS, startAngle, endAngle, Fade(GREEN, 0.25f));

        // debug: draw forward direction
        __DebugDrawVector(car.position, forward, CAR_DETECTION_RADIUS, 2);

        // debug: draw hands
        Vector2 arcA = Vector2Rotate(forward, startAngle - forwardAngleDeg);
        Vector2 arcB = Vector2Rotate(forward, endAngle - forwardAngleDeg);
        __DebugDrawVector(car.position, arcA, CAR_DETECTION_RADIUS, 2, YELLOW);
        __DebugDrawVector(car.position, arcB, CAR_DETECTION_RADIUS, 2, YELLOW);
    }

    if (!CanCarPass(state.trafficLightGroup, car) || !m_canCarMove(car, state))
        StopCar(car);
    else
        ResumeCar(car);

    m_updateVelocity(car, state);
}

void DrawCar(const Car &car)
{
    Rectangle carRect = {
        .x = car.position.x - car.size.x / 2,
        .y = car.position.y - car.size.y / 2,
        .width = car.size.x,
        .height = car.size.y //
    };
    DrawRectangleRec(carRect, car.color);
}

void SetCarVelocity(Car &car, Vector2 newVelocity)
{
    car.desiredVelocity = newVelocity;
}

Vector2 GetCarVelocity(const Car &car)
{
    return car.desiredVelocity;
}

void StopCar(Car &car)
{
    car._velocity = {0.f, 0.f};
}

void ResumeCar(Car &car)
{
    car._velocity = car.desiredVelocity;
}
