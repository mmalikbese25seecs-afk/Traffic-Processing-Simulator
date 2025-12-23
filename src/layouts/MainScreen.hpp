#pragma once

#include <vector>
#include <raylib.h>

#include "ConfigTree.hpp"

#include "Car.hpp"
#include "TrafficLight.hpp"

constexpr float START_OFFSET = -10.f;
constexpr float SPACING = 50.f;
constexpr float TRAFFIC_LIGHT_WAIT_POS_OFFSET = 20.f;

struct GameState
{
    TrafficLightGroup trafficLightGroup;
    float carsPerSecond = 0.5f;
    uint16_t _lastAddedCarId = 0;

    std::vector<Car> cars;
    float deltaTime = 0.f;

    bool paused = false;

    Node rootConfigNode;
};

void InitGameState(GameState &state);
void UpdateGameState(GameState &state);
void RestartGameState(GameState &state);

void PauseGameState(GameState &state);
void ResumeGameState(GameState &state);

void DrawMainScreen(const GameState &state);
