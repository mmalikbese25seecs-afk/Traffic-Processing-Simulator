// TrafficLight.hpp (updated)
#pragma once

#include <raylib.h>

#include <array>
#include <cstdint>
#include <unordered_set>

struct Car;
struct GameState;

// debug: allow cars to ignore lights for visual/testing
constexpr bool DEBUG_TRAFFIC_LIGHT_CAR_CAN_PASS = true;
constexpr bool DEBUG_TRAFFIC_LIGHT_CAR_PASSED = true;

// traffic light graphics constants
constexpr float TRAFFIC_LIGHT_RADIUS = 10.f;
constexpr Color TRAFFIC_LIGHT_ON_COLOR = GREEN;
constexpr Color TRAFFIC_LIGHT_OFF_COLOR = RED;
constexpr Color TRAFFIC_LIGHT_BG_COLOR = BLACK;
constexpr float TRAFFIC_LIGHT_BG_PADDING = 5.f;
// distance from traffic light to stop car
constexpr float TRAFFIC_LIGHT_CAR_DETECTION_RANGE = 50.f;
constexpr float TRAFFIC_LIGHT_CAP_DISTANCE = 25.f;

// realistic timing (seconds)
constexpr float TRAFFIC_LIGHT_GREEN_DURATION = 10.f;
constexpr float TRAFFIC_LIGHT_YELLOW_DURATION = 3.f;
constexpr float TRAFFIC_LIGHT_ALL_RED_DURATION = 0.5f;

// colors
constexpr Color TRAFFIC_LIGHT_YELLOW_COLOR = YELLOW;

enum class TrafficLightState
{
    STOP, // RED
    WAIT, // YELLOW
    GO,   // GREEN
};

enum class TrafficLightGroupPhase
{
    GREEN_PHASE,
    YELLOW_PHASE,
    ALL_RED_PHASE,
};

struct TrafficLight
{
    Vector2 position;
    Vector2 direction;
    // position where cars should stop when orange light
    Vector2 waitingPosition;
    TrafficLightState state;
    std::unordered_set<uint16_t> carsPassed; // IDs of cars that have passed this light
};

struct TrafficLightGroup
{
    std::array<TrafficLight, 4> trafficLights;
    // is true when lights at [0, 2] are on; false when [1, 3] are on
    bool currentGroup = false;

    // current phase of the group (green -> yellow -> all-red -> green-other)
    TrafficLightGroupPhase phase = TrafficLightGroupPhase::GREEN_PHASE;
    // when the current phase started (GetTime())
    double phaseStartTime = 0.0;
};

void UpdateTrafficLights(GameState &state);
void SwitchTrafficLights(TrafficLightGroup &group);
void DrawTrafficLightGroup(const TrafficLightGroup &light);

void ForceUpdateTrafficLights(GameState &state);

bool CanCarPass(TrafficLightGroup &group, const Car &car);
