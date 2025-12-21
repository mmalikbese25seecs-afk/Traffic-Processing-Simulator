#pragma once

#include <vector>
#include <raylib.h>

#include "Car.hpp"
#include "TrafficLight.hpp"

constexpr float START_OFFSET = -10.f;
constexpr float SPACING = 50.f;
constexpr float TRAFFIC_LIGHT_WAIT_POS_OFFSET = 20.f;

constexpr float MAIN_SCREEN_CAR_SKIP_SPAWN_CHANCE = 0.5f;
constexpr bool MAIN_SCREEN_DEBUG_DRAW_TRAFFIC_LIGHT_POSITIONS = false;

struct GameState
{
    TrafficLightGroup trafficLightGroup;
    float carsPerSecond = 0.5f;

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
