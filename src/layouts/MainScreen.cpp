#include "MainScreen.hpp"

#include "WindowConfig.hpp"

void m_drawRoad()
{
}

void layouts::drawMainScreen()
{
    // left road -------------------------------------------------------
    Rectangle leftRoadRect = Rectangle{
        .x = 0,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_SIZE //
    };

    // road outer line top
    Rectangle leftRoadOuterUpperLineRect = Rectangle{
        .x = 0,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 + ROAD_OUTER_LINE_PADDING,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_OUTER_LINE_WIDTH //
    };
    Rectangle leftRoadOuterLowerLineRect = Rectangle{
        .x = 0,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2 - ROAD_OUTER_LINE_PADDING - ROAD_OUTER_LINE_WIDTH,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_OUTER_LINE_WIDTH //
    };

    // line in middle
    Rectangle leftRoadMiddleLineRect = Rectangle{
        .x = 0,
        .y = CFG::WINDOW_CENTER.y - ROAD_MIDDLE_LINE_WIDTH / 2,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_MIDDLE_LINE_WIDTH //
    };

    // right road ------------------------------------------------------
    Rectangle rightRoadRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_SIZE //
    };

    // outer lines
    Rectangle rightRoadOuterUpperLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 + ROAD_OUTER_LINE_PADDING,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_OUTER_LINE_WIDTH //
    };
    Rectangle rightRoadOuterLowerLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2 - ROAD_OUTER_LINE_PADDING - ROAD_OUTER_LINE_WIDTH,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_OUTER_LINE_WIDTH //
    };

    // middle line
    Rectangle rightRoadMiddleLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y - ROAD_MIDDLE_LINE_WIDTH / 2,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_MIDDLE_LINE_WIDTH //
    };

    // top road --------------------------------------------------------
    Rectangle topRoadRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .y = 0,
        .width = ROAD_SIZE,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    // outer lines
    Rectangle topRoadOuterLeftLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2 + ROAD_OUTER_LINE_PADDING,
        .y = 0,
        .width = ROAD_OUTER_LINE_WIDTH,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    Rectangle topRoadOuterRightLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2 - ROAD_OUTER_LINE_PADDING - ROAD_OUTER_LINE_WIDTH,
        .y = 0,
        .width = ROAD_OUTER_LINE_WIDTH,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    // middle line
    Rectangle topRoadMiddleLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_MIDDLE_LINE_WIDTH / 2,
        .y = 0,
        .width = ROAD_MIDDLE_LINE_WIDTH,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    // bottom road -----------------------------------------------------
    Rectangle bottomRoadRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2,
        .width = ROAD_SIZE,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    // outer lines
    Rectangle bottomRoadOuterLeftLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2 + ROAD_OUTER_LINE_PADDING,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2,
        .width = ROAD_OUTER_LINE_WIDTH,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    Rectangle bottomRoadOuterRightLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2 - ROAD_OUTER_LINE_PADDING - ROAD_OUTER_LINE_WIDTH,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2,
        .width = ROAD_OUTER_LINE_WIDTH,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    // middle line
    Rectangle bottomRoadMiddleLineRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_MIDDLE_LINE_WIDTH / 2,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2,
        .width = ROAD_MIDDLE_LINE_WIDTH,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    // center intersection box -----------------------------------------
    Rectangle centerRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2,
        .width = ROAD_SIZE,
        .height = ROAD_SIZE //
    };

    DrawRectangleRec(leftRoadRect, ROAD_COLOR);
    DrawRectangleRec(leftRoadOuterUpperLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(leftRoadOuterLowerLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(leftRoadMiddleLineRect, ROAD_MIDDLE_LINE_COLOR);

    DrawRectangleRec(rightRoadRect, ROAD_COLOR);
    DrawRectangleRec(rightRoadOuterUpperLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(rightRoadOuterLowerLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(rightRoadMiddleLineRect, ROAD_MIDDLE_LINE_COLOR);

    DrawRectangleRec(topRoadRect, ROAD_COLOR);
    DrawRectangleRec(topRoadOuterLeftLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(topRoadOuterRightLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(topRoadMiddleLineRect, ROAD_MIDDLE_LINE_COLOR);

    DrawRectangleRec(bottomRoadRect, ROAD_COLOR);
    DrawRectangleRec(bottomRoadOuterLeftLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(bottomRoadOuterRightLineRect, ROAD_OUTER_LINE_COLOR);
    DrawRectangleRec(bottomRoadMiddleLineRect, ROAD_MIDDLE_LINE_COLOR);

    DrawRectangleRec(centerRect, ROAD_COLOR);

    Vector2 topLeftCorner = {
        CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    m_drawL(topLeftCorner, ROAD_OUTER_LINE_PADDING, ROAD_OUTER_LINE_WIDTH, true, false, ROAD_OUTER_LINE_COLOR);
    Vector2 topRightCorner = {
        CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    m_drawL(topRightCorner, ROAD_OUTER_LINE_PADDING, ROAD_OUTER_LINE_WIDTH, true, true, ROAD_OUTER_LINE_COLOR);
    Vector2 bottomLeftCorner = {
        CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        CFG::WINDOW_CENTER.y + ROAD_SIZE / 2 //
    };
    m_drawL(bottomLeftCorner, ROAD_OUTER_LINE_PADDING, ROAD_OUTER_LINE_WIDTH, false, false, ROAD_OUTER_LINE_COLOR);
    Vector2 bottomRightCorner = {
        CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        CFG::WINDOW_CENTER.y + ROAD_SIZE / 2 //
    };
    m_drawL(bottomRightCorner, ROAD_OUTER_LINE_PADDING, ROAD_OUTER_LINE_WIDTH, false, true, ROAD_OUTER_LINE_COLOR);
}

void layouts::m_drawL(Vector2 center, float perpendicularDistanceToEdge, float width, bool facesTop, bool facesLeft, Color color)
{
    // horizontal part
    Rectangle horizontalRect = Rectangle{
        .x = center.x + (facesLeft ? -perpendicularDistanceToEdge - width : 0),
        .y = center.y + (facesTop ? perpendicularDistanceToEdge : -perpendicularDistanceToEdge - width),
        .width = perpendicularDistanceToEdge + width, // + width to cover corner
        .height = width                               //
    };
    DrawRectangleRec(horizontalRect, color);

    // vertical part
    Rectangle verticalRect = Rectangle{
        .x = center.x + (facesLeft ? -perpendicularDistanceToEdge - width : perpendicularDistanceToEdge),
        .y = center.y + (!facesTop ? -perpendicularDistanceToEdge : 0),
        .width = width,
        .height = perpendicularDistanceToEdge // no corner compensation for vertical
    };
    DrawRectangleRec(verticalRect, color);
}
