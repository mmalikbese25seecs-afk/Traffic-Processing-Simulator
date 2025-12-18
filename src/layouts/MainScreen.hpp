#pragma once

#include <vector>
#include <raylib.h>

namespace layouts
{
    // road
    constexpr int ROAD_SIZE = 100;
    constexpr Color ROAD_COLOR = GRAY;

    constexpr int ROAD_OUTER_LINE_PADDING = 4;
    constexpr int ROAD_OUTER_LINE_WIDTH = ROAD_SIZE / 20;
    constexpr Color ROAD_OUTER_LINE_COLOR = WHITE;

    constexpr int ROAD_MIDDLE_LINE_WIDTH = ROAD_SIZE / 20;
    constexpr Color ROAD_MIDDLE_LINE_COLOR = YELLOW;

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

    struct GameState
    {
        std::vector<TrafficLight> trafficLights;
    };

    void initGameState(GameState &state);
    void updateGameState(GameState &state);

    void m_drawRoads();
    void drawMainScreen(const GameState &state);

    // utils
    void m_drawTrafficLightSymbol(Vector2 center, float size, Color color);
    void m_drawLShape(Vector2 center, float perpendicularDistanceToEdge, float width, bool facesTop, bool facesLeft, Color color);
}
