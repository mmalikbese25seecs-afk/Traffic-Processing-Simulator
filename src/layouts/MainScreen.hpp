#pragma once

#include <vector>
#include <raylib.h>

#include "TrafficLight.hpp"
#include "Roads.hpp"
#include "Car.hpp"

struct Car;

struct GameState
{
    TrafficLightGroup trafficLightGroup;

    std::vector<Car> cars;
    float deltaTime = 0.0f;
};

void InitGameState(GameState &state);
void UpdateGameState(GameState &state);

void DrawMainScreen(const GameState &state);
