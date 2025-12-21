#pragma once
#include <raylib.h>
#include <queue>
#include <iostream>

#include "VectorMath.hpp"

struct __DebugLine
{
    Vector2 start;
    Vector2 end;
    int thickness = 5;
    Color color;
};

struct __DebugPoint
{
    Vector2 position;
    float radius = 5.f;
    Color color;
};

struct __DebugCircleArc
{
    Vector2 center;
    float radius;
    float startAngle = 0.f;
    float endAngle = 360.f;
    Color color;
};

inline std::queue<__DebugLine> g_DebugLines;
inline std::queue<__DebugPoint> g_DebugPoints;
inline std::queue<__DebugCircleArc> g_DebugCircleArcs;

/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param scale [OPTIONAL] Default `1.f`
inline void __DebugDrawVector(const Vector2 &origin, const Vector2 &vector, float scale = 1.f, int thickness = 5, Color color = RED)
{
    // using queue because we process then delete
    __DebugLine line;
    line.start = origin;
    line.end = {origin.x + vector.x * scale, origin.y + vector.y * scale};
    line.color = color;
    line.thickness = thickness;
    g_DebugLines.push(line);
}

/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param radius [OPTIONAL] Default `5.f`
inline void __DebugDrawPoint(const Vector2 &position, float radius = 5.f, Color color = RED)
{
    __DebugPoint point;
    point.position = position;
    point.color = color;
    point.radius = radius;
    g_DebugPoints.push(point);
}

/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
inline void __DebugDrawCircleArc(
    const Vector2 &center,
    float radius,
    float startAngle = 0.f,
    float endAngle = 360.f,
    Color color = RED)
{
    float start = AngleNormalizeDeg(startAngle);
    float end = AngleNormalizeDeg(endAngle);

    std::cout << "Debug Draw Circle Arc: start=" << start << " end=" << end << "\n";

    // Normal case: no wraparound
    if (start <= end)
    {
        __DebugCircleArc circle;
        circle.center = center;
        circle.radius = radius;
        circle.startAngle = start;
        circle.endAngle = end;
        circle.color = color;
        g_DebugCircleArcs.push(circle);
    }
    else
    {
        // Wraparound case: split into two arcs

        {
            __DebugCircleArc circle;
            circle.center = center;
            circle.radius = radius;
            circle.startAngle = start;
            circle.endAngle = 360.0f;
            circle.color = color;
            g_DebugCircleArcs.push(circle);
        }

        {
            __DebugCircleArc circle;
            circle.center = center;
            circle.radius = radius;
            circle.startAngle = 0.0f;
            circle.endAngle = end;
            circle.color = color;
            g_DebugCircleArcs.push(circle);
        }
    }
}

inline void __ProcessDebugDraws()
{
    // draw lines of each frame, the next frame clears the previous line
    while (!g_DebugCircleArcs.empty())
    {
        __DebugCircleArc circle = g_DebugCircleArcs.front();
        DrawCircleSector(circle.center, circle.radius, circle.startAngle, circle.endAngle, 36, circle.color);
        g_DebugCircleArcs.pop();
    }

    while (!g_DebugLines.empty())
    {
        __DebugLine line = g_DebugLines.front();
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