#pragma once

#include <vector>
#include <raylib.h>

#include "TrafficLight.hpp"
#include "Roads.hpp"

struct GameState
{
    TrafficLightGroup trafficLightGroup;
};

void initGameState(GameState &state);
void updateGameState(GameState &state);

void drawMainScreen(const GameState &state);
