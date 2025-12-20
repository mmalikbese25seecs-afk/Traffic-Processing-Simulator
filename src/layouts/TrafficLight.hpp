#pragma once

#include <array>
#include <raylib.h>

// traffic light
constexpr float TRAFFIC_LIGHT_RADIUS = 10.0f;
constexpr Color TRAFFIC_LIGHT_ON_COLOR = GREEN;
constexpr Color TRAFFIC_LIGHT_OFF_COLOR = RED;

struct TrafficLight
{
    Vector2 position;
    Vector2 direction;
    bool isOn;
};

struct TrafficLightGroup
{
    std::array<TrafficLight, 4> trafficLights;
    // is true when lights at 0, 2 are on; false when 1, 3 are on
    bool currentGroup = false;
};

void switchTrafficLights(TrafficLightGroup &group);
void m_drawTrafficLightSymbol(Vector2 center, float size, Color color);
