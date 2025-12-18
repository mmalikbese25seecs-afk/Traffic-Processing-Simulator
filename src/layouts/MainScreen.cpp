#include "MainScreen.hpp"

#include "WindowConfig.hpp"

void layouts::drawMainScreen()
{
    // center intersection box
    Rectangle centerRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2,
        .width = ROAD_SIZE,
        .height = ROAD_SIZE //
    };

    // left road
    Rectangle leftRoadRect = Rectangle{
        .x = 0,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_SIZE //
    };
    // right road
    Rectangle rightRoadRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x + ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2,
        .width = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .height = ROAD_SIZE //
    };
    // top road
    Rectangle topRoadRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .y = 0,
        .width = ROAD_SIZE,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    // bottom road
    Rectangle bottomRoadRect = Rectangle{
        .x = CFG::WINDOW_CENTER.x - ROAD_SIZE / 2,
        .y = CFG::WINDOW_CENTER.y + ROAD_SIZE / 2,
        .width = ROAD_SIZE,
        .height = CFG::WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };

    DrawRectangleRec(centerRect, ROAD_COLOR);
    DrawRectangleRec(leftRoadRect, ROAD_COLOR);
    DrawRectangleRec(rightRoadRect, ROAD_COLOR);
    DrawRectangleRec(topRoadRect, ROAD_COLOR);
    DrawRectangleRec(bottomRoadRect, ROAD_COLOR);
}
