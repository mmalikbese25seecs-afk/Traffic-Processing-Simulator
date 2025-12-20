#pragma once
#include <raylib.h>
#include <queue>

struct __DebugLine
{
    Vector2 start;
    Vector2 end;
    Color color;
    int thickness = 5;
};

struct __DebugPoint
{
    Vector2 position;
    Color color;
    float radius = 5.f;
};

inline std::queue<__DebugLine> g_DebugLines;
inline std::queue<__DebugPoint> g_DebugPoints;
/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param scale [OPTIONAL] Default `1.f`
inline void __DebugDrawVector(const Vector2 &origin, const Vector2 &vector, Color color = RED, float scale = 1.f)
{
    // using queue because we process then delete
    __DebugLine line;
    line.start = origin;
    line.end = {origin.x + vector.x * scale, origin.y + vector.y * scale};
    line.color = color;
    g_DebugLines.push(line);
}

/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param radius [OPTIONAL] Default `5.f`
inline void __DebugDrawPoint(const Vector2 &position, Color color = RED, float radius = 5.f)
{
    __DebugPoint point;
    point.position = position;
    point.color = color;
    point.radius = radius;
    g_DebugPoints.push(point);
}

inline void __ProcessDebugDraws()
{
    // draw lines of each frame, the next frame clears the previous line
    while (!g_DebugLines.empty())
    {
        __DebugLine line = g_DebugLines.front();
        // DrawLine(static_cast<int>(line.start.x), static_cast<int>(line.start.y),
        //          static_cast<int>(line.end.x), static_cast<int>(line.end.y),
        //          line.color);
        DrawLineEx(line.start, line.end, static_cast<float>(line.thickness), line.color);
        g_DebugLines.pop();
    }
    while (!g_DebugPoints.empty())
    {
        __DebugPoint point = g_DebugPoints.front();
        DrawCircleV(point.position, point.radius, point.color);
        g_DebugPoints.pop();
    }
}