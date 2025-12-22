#pragma once
#include <raylib.h>
#include <queue>
#include <iostream>

#include "VectorMath.hpp"

#define DEBUG_DRAW_ENABLED 1

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

struct __DebugText
{
    Vector2 position;
    std::string text;
    int fontSize = 20;
    bool centered = false;
    Color color;
};

inline std::queue<__DebugLine> g_DebugLines;
inline std::queue<__DebugPoint> g_DebugPoints;
inline std::queue<__DebugCircleArc> g_DebugCircleArcs;
inline std::queue<__DebugText> g_DebugTexts;

/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param radius [OPTIONAL] Default `5.f`
inline void __DebugDrawPoint(const Vector2 &position, float radius = 5.f, Color color = RED)
{
#if DEBUG_DRAW_ENABLED
    __DebugPoint point;
    point.position = position;
    point.color = color;
    point.radius = radius;
    g_DebugPoints.push(point);
#endif
}

/// @brief Draws `vector` at given `origin`; Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param scale [OPTIONAL] Default `1.f`
inline void __DebugDrawVectorAt(const Vector2 &origin, const Vector2 &vector, float scale = 1.f, int thickness = 5, bool highlightPoints = true, Color color = RED)
{
#if DEBUG_DRAW_ENABLED
    // using queue because we process then delete
    __DebugLine line;
    line.start = origin;
    line.end = {origin.x + vector.x * scale, origin.y + vector.y * scale};
    line.color = color;
    line.thickness = thickness;
    g_DebugLines.push(line);

    if (highlightPoints)
    {
        __DebugDrawPoint(origin, thickness * 2.f, color);
        __DebugDrawPoint(line.end, thickness * 2.f, color);
    }
#endif
}

/// @brief Draws line from point `a` to `b`; Call every frame
/// @param color [OPTIONAL] Default `RED`
inline void __DebugDrawVectorAB(const Vector2 &a, const Vector2 &b, int thickness = 5, bool highlightPoints = true, Color color = RED)
{
#if DEBUG_DRAW_ENABLED
    __DebugLine line;
    line.start = a;
    line.end = b;
    line.color = color;
    line.thickness = thickness;
    g_DebugLines.push(line);

    if (highlightPoints)
    {
        __DebugDrawPoint(a, thickness * 2.f, color);
        __DebugDrawPoint(b, thickness * 2.f, color);
    }
#endif
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
#if DEBUG_DRAW_ENABLED
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
#endif
}
/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param fontSize [OPTIONAL] Default `20`
inline void __DebugDrawText(const Vector2 &position, const std::string &text, int fontSize = 20, bool centered = false, Color color = MAGENTA)
{
#if DEBUG_DRAW_ENABLED
    __DebugText debugText;
    debugText.position = position;
    debugText.text = text;
    debugText.fontSize = fontSize;
    debugText.color = color;
    debugText.centered = centered;
    g_DebugTexts.push(debugText);
#endif
}

inline void __ProcessDebugDraws()
{
#if DEBUG_DRAW_ENABLED
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

    while (!g_DebugTexts.empty())
    {
        __DebugText debugText = g_DebugTexts.front();
        Vector2 textSize = MeasureTextEx(GetFontDefault(), debugText.text.c_str(), static_cast<float>(debugText.fontSize), 1.f);
        Vector2 drawPos = debugText.position;
        if (debugText.centered)
        {
            drawPos.x -= textSize.x / 2.f;
            drawPos.y -= textSize.y / 2.f;
        }
        DrawTextEx(GetFontDefault(), debugText.text.c_str(), drawPos, static_cast<float>(debugText.fontSize), 1.f, debugText.color);

        // draw background box
        DrawRectangle(
            static_cast<int>(drawPos.x) - 2,
            static_cast<int>(drawPos.y) - 2,
            static_cast<int>(textSize.x) + 4,
            static_cast<int>(textSize.y) + 4,
            Fade(BLACK, 0.5f) //
        );
        
        g_DebugTexts.pop();
    }
#endif
}