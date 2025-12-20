#include "MainScreen.hpp"

#include "WindowConfig.hpp"
#include "Roads.hpp"

void InitGameState(GameState &state)
{
    // traffic lights
    Vector2 topRoadLightPos = {
        WINDOW_CENTER.x,
        WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    Vector2 leftRoadLightPos = {
        WINDOW_CENTER.x - ROAD_SIZE / 2,
        WINDOW_CENTER.y //
    };
    Vector2 bottomRoadLightPos = {
        WINDOW_CENTER.x,
        WINDOW_CENTER.y + ROAD_SIZE / 2 //
    };
    Vector2 rightRoadLightPos = {
        WINDOW_CENTER.x + ROAD_SIZE / 2,
        WINDOW_CENTER.y //
    };

    state.trafficLightGroup.trafficLights[0] = TrafficLight{topRoadLightPos, {0, 1}, true};
    state.trafficLightGroup.trafficLights[1] = TrafficLight{leftRoadLightPos, {1, 0}, false};
    state.trafficLightGroup.trafficLights[2] = TrafficLight{bottomRoadLightPos, {0, -1}, false};
    state.trafficLightGroup.trafficLights[3] = TrafficLight{rightRoadLightPos, {-1, 0}, false};
    state.trafficLightGroup.currentGroup = true;

    // cars
    Vector2 carStartPos = {
        WINDOW_CENTER.x,
        WINDOW_SIZE.y + 200.f //
    };
    // for direction car is facing towards center
    Vector2 carVelocity = {0, -50}; // moving upwards
    state.cars.push_back(Car{carStartPos, carVelocity, BLUE});
}

void m_updateTrafficLights(GameState &state)
{
    const double time = GetTime();
    const int interval = 1; // seconds

    static int lastTick = -1;
    const int currentTick = static_cast<int>(time) / interval;

    if (currentTick == lastTick)
        return;

    // code below runs once per interval
    SwitchTrafficLights(state.trafficLightGroup);

    lastTick = currentTick;
}

void UpdateGameState(GameState &state)
{
    state.deltaTime = GetFrameTime();

    m_updateTrafficLights(state);

    for (auto &car : state.cars)
    {
        UpdateCar(car, state);
    }
}

void DrawMainScreen(const GameState &state)
{
    DrawRoads();
    DrawTrafficLightGroup(state.trafficLightGroup);

    for (const auto &car : state.cars)
    {
        DrawCar(car);
    }
}
