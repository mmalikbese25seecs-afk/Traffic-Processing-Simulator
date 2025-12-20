#include "MainScreen.hpp"

#include "WindowConfig.hpp"

void initGameState(GameState &state)
{
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

    // add traffic lights
    state.trafficLightGroup.trafficLights[0] = TrafficLight{topRoadLightPos, {0, -1}, true};
    state.trafficLightGroup.trafficLights[1] = TrafficLight{leftRoadLightPos, {-1, 0}, false};
    state.trafficLightGroup.trafficLights[2] = TrafficLight{bottomRoadLightPos, {0, 1}, false};
    state.trafficLightGroup.trafficLights[3] = TrafficLight{rightRoadLightPos, {1, 0}, false};
}

void updateGameState(GameState &state)
{
    const double time = GetTime();
    const int interval = 1; // seconds

    static int lastTick = -1;
    const int currentTick = static_cast<int>(time) / interval;

    if (currentTick == lastTick)
        return;

    // code below runs once per interval
    switchTrafficLights(state.trafficLightGroup);

    lastTick = currentTick;
}

void drawMainScreen(const GameState &state)
{
    DrawRoads();

    for (const auto &trafficLight : state.trafficLightGroup.trafficLights)
    {
        Color trafficLightColor = trafficLight.isOn ? TRAFFIC_LIGHT_ON_COLOR : TRAFFIC_LIGHT_OFF_COLOR;
        m_drawTrafficLightSymbol(trafficLight.position, TRAFFIC_LIGHT_RADIUS, trafficLightColor);
    }
}
