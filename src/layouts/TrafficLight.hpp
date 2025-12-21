#pragma once

#include <array>
#include <raylib.h>

struct Car;
struct GameState;

// traffic light
constexpr float TRAFFIC_LIGHT_RADIUS = 10.f;
constexpr Color TRAFFIC_LIGHT_ON_COLOR = GREEN;
constexpr Color TRAFFIC_LIGHT_OFF_COLOR = RED;
constexpr Color TRAFFIC_LIGHT_BG_COLOR = BLACK;
constexpr float TRAFFIC_LIGHT_BG_PADDING = 5.f;
// distance from traffic light to stop car
constexpr float TRAFFIC_LIGHT_STOP_DISTANCE = 50.f;
constexpr float TRAFFIC_LIGHT_CAP_DISTANCE = 25.f;
constexpr int TRAFFIC_LIGHT_SWITCH_INTERVAL = 5.f; // seconds

struct TrafficLight
{
    Vector2 position;
    Vector2 direction;
    bool isOn;
};

struct TrafficLightGroup
{
    std::array<TrafficLight, 4> trafficLights;
    // is true when lights at [0, 2] are on; false when [1, 3] are on
    bool currentGroup = false;
};

void UpdateTrafficLights(GameState &state);
void SwitchTrafficLights(TrafficLightGroup &group);
void DrawTrafficLightGroup(const TrafficLightGroup &light);

void ForceUpdateTrafficLights(GameState &state);

bool CanCarPass(const TrafficLightGroup &trafficLight, const Car &car);
