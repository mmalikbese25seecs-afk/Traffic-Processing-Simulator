#pragma once

#include <vector>
#include <raylib.h>

#include "Car.hpp"
#include "TrafficLight.hpp"

constexpr int NUM_CARS_PER_DIRECTION = 5;
constexpr float START_OFFSET = -10.f;
constexpr float SPACING = 50.f;

struct GameState
{
    TrafficLightGroup trafficLightGroup;

    std::vector<Car> cars;
    float deltaTime = 0.f;
};

void InitGameState(GameState &state);
void UpdateGameState(GameState &state);
void RestartGameState(GameState &state);

void DrawMainScreen(const GameState &state);
