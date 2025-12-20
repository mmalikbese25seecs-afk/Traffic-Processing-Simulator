#include "Car.hpp"

#include <iostream>
#include <cmath>

#include "Debug.hpp"


void m_updateVelocity(Car &car, const GameState &state)
{
    car.position.x += car.velocity.x * state.deltaTime;
    car.position.y += car.velocity.y * state.deltaTime;
}

void UpdateCar(Car &car, const GameState &state)
{
    __DebugDrawVector(car.position, car.velocity, MAGENTA);

    // check if the direction of travel is obeyed by traffic light from `state.trafficLightGroup`

    // get forward direction
    Vector2 forwardDir = {0, 0};
    float len = sqrtf(car.desiredVelocity.x * car.desiredVelocity.x +
                      car.desiredVelocity.y * car.desiredVelocity.y);
    if (len > 0.0001f)
    {
        forwardDir.x = car.desiredVelocity.x / len;
        forwardDir.y = car.desiredVelocity.y / len;
    }

    // dot the forward direction with the traffic light direction
    // if dot > 0, moving towards the traffic light
    bool stop = false;
    for (const auto &trafficLight : state.trafficLightGroup.trafficLights)
    {
        float dot = forwardDir.x * trafficLight.direction.x + forwardDir.y * trafficLight.direction.y;
        if (dot > 0 && !trafficLight.isOn)
        {
            stop = true; // red light ahead
            break;
        }
    }

    // check if the traffic light is red
    // if red, stop the car (set velocity to zero)
    if (stop)
        car.velocity = {0, 0};
    else
        car.velocity = car.desiredVelocity;

    m_updateVelocity(car, state);
}

void DrawCar(const Car &car)
{
    const float carWidth = 20.0f;
    const float carHeight = 40.0f;
    Rectangle carRect = {
        .x = car.position.x - carWidth / 2,
        .y = car.position.y - carHeight / 2,
        .width = carWidth,
        .height = carHeight //
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
