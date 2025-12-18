#pragma once

#include <raylib.h>

namespace layouts
{
    constexpr int ROAD_SIZE = 100;
    constexpr Color ROAD_COLOR = GRAY;

    constexpr int ROAD_OUTER_LINE_PADDING = 4;
    constexpr int ROAD_OUTER_LINE_WIDTH = ROAD_SIZE / 20;
    constexpr Color ROAD_OUTER_LINE_COLOR = WHITE;

    constexpr int ROAD_MIDDLE_LINE_WIDTH = ROAD_SIZE / 20;
    constexpr Color ROAD_MIDDLE_LINE_COLOR = YELLOW;

    void drawMainScreen();

    void m_drawL(Vector2 center, float perpendicularDistanceToEdge, float width, bool facesTop, bool facesLeft, Color color);
}
