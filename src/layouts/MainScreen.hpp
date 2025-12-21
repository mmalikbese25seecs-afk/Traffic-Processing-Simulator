#pragma once

#include <vector>
#include <raylib.h>

#include "Car.hpp"
#include "TrafficLight.hpp"

constexpr float START_OFFSET = -10.f;
constexpr float SPACING = 50.f;

struct GameState
{
    TrafficLightGroup trafficLightGroup;
    float carsPerSecond = 1.f;

    std::vector<Car> cars;
    float deltaTime = 0.f;

    bool paused = false;
};

void InitGameState(GameState &state);
void UpdateGameState(GameState &state);
void RestartGameState(GameState &state);
void PauseGameState(GameState &state);
void ResumeGameState(GameState &state);

void DrawMainScreen(const GameState &state);
