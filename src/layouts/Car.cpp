#include "Car.hpp"

#include <iostream>
#include <cmath>
#include <variant>

#include "MainScreen.hpp"
#include "Debug.hpp"
#include "VectorMath.hpp"

void m_updateVelocity(Car &car, const GameState &state)
{
    car.position.x += car._velocity.x * state.deltaTime;
    car.position.y += car._velocity.y * state.deltaTime;
}

bool m_checkCarCollision(Car &car, GameState &state)
{
    float detectionRadius = GetConfigFloat(state.rootConfigNode, "radius", 50.f);
    float detectionAngle = GetConfigInt(state.rootConfigNode, "angle_deg", 45);

    bool debugOtherCars = GetConfigBool(state.rootConfigNode, "debug_car_detection_other_cars", false);

    for (const auto &otherCar : state.cars)
    {
        // skip self
        if (otherCar.id == car.id)
            continue;

        float distance = Vector2Distance(car.position, otherCar.position);
        if (distance < detectionRadius)
        {
            if (debugOtherCars)
            {
                __DebugDrawVectorAB(car.position, otherCar.position, 2, true, MAGENTA);
            }

            bool isCarInFront = Vector2AfterPoint(otherCar.position, car.position, car.desiredVelocity, detectionAngle);

            // other car is in front within detection angle
            if (isCarInFront)
            {
                car.state = otherCar.state;
                return false;
            }
        }
    }

    // no car in front
    return true;
}

void m_DebugDrawCarArc(const Car &car, const Node &rootConfigNode)
{
    bool debugArc = GetConfigBool(rootConfigNode, "debug_car_detection_arc", false);
    if (!debugArc)
        return;

    if (Vector2IsZero(car.desiredVelocity))
        return;

    // cannot determine direction
    Vector2 forward = car.desiredVelocity;
    Vector2Normalize(forward);

    // compute forward angle in degrees (raylib space)
    float forwardAngleDeg = atan2f(forward.y, forward.x) * RAD2DEG;

    float detectionAngle = GetConfigInt(rootConfigNode, "angle_deg", 45);
    float detectionRadius = GetConfigFloat(rootConfigNode, "radius", 50.f);

    float startAngle = forwardAngleDeg - detectionAngle;
    float endAngle = forwardAngleDeg + detectionAngle;

    // debug: draw detection arc
    __DebugDrawCircleArc(car.position, detectionRadius, startAngle, endAngle, Fade(GREEN, 0.25f));

    // debug: draw forward direction
    __DebugDrawVectorAt(car.position, forward, detectionRadius, 2);

    // debug: draw hands
    Vector2 arcA = Vector2Rotate(forward, startAngle - forwardAngleDeg);
    Vector2 arcB = Vector2Rotate(forward, endAngle - forwardAngleDeg);
    __DebugDrawVectorAt(car.position, arcA, detectionRadius, 2, false, YELLOW);
    __DebugDrawVectorAt(car.position, arcB, detectionRadius, 2, false, YELLOW);
}

void UpdateCar(Car &car, GameState &state)
{
    if (!TrafficLightUpdateCarState(state.trafficLightGroup, car) || !m_checkCarCollision(car, state))
    {
        car._velocity = {0.f, 0.f};
    }
    else
    {
        car._velocity = car.desiredVelocity;
    }

    m_updateVelocity(car, state);

    // only update rotation if velocity is non-zero
    const Vector2 &vel = car._velocity;
    if (!Vector2IsZero(vel))
    {
        car.rotation = AngleToDegrees(atan2f(vel.y, vel.x));
    }

    m_DebugDrawCarArc(car, state.rootConfigNode);
}

void DrawCar(const Car &car, const GameState& state)
{
    Rectangle carRect = {
        .x = car.position.x,
        .y = car.position.y,
        .width = car.size.x,
        .height = car.size.y //
    };
    // top middle
    Vector2 origin = {
        car.size.x / 2.f,
        car.size.y / 2.f //
    };
    DrawRectanglePro(carRect, origin, car.rotation, car.color);

    if (GetConfigBool(state.rootConfigNode, "debug_car_state", true))
    {
        // draw green point if moving, orange if waiting
        Color stateColor = (car.state == CarState::MOVING) ? GREEN : ORANGE;
        __DebugDrawPoint(car.position, 5.f, stateColor);
    }
}
