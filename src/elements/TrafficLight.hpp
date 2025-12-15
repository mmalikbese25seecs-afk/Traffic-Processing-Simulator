#pragma once
#include <raylib.h>

enum class TrafficLightState
{
    TRED,
    TORANGE,
    TGREEN,
};

struct TrafficLight
{
    Vector2 position;
    Vector2 size;
    Color currentColor = RED;
    TrafficLightState currentState = TrafficLightState::TRED;
};

void UpdateTrafficLight(TrafficLight &trafficLight);
void DrawTrafficLight(const TrafficLight &trafficLight);
