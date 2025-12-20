#include "Car.hpp"

#include <iostream>
#include <cmath>

#include "MainScreen.hpp"
#include "Debug.hpp"

void m_updateVelocity(Car &car, const GameState &state)
{
    car.position.x += car._velocity.x * state.deltaTime;
    car.position.y += car._velocity.y * state.deltaTime;
}

void UpdateCar(Car &car, const GameState &state)
{
    if (!CanCarPass(state.trafficLightGroup, car))
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
