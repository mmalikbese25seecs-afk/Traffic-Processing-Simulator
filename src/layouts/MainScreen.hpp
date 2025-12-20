#pragma once

#include <vector>
#include <raylib.h>

#include "Car.hpp"
#include "TrafficLight.hpp"

struct GameState
{
    TrafficLightGroup trafficLightGroup;

    std::vector<Car> cars;
    float deltaTime = 0.f;
};

void InitGameState(GameState &state);
void UpdateGameState(GameState &state);

void DrawMainScreen(const GameState &state);
