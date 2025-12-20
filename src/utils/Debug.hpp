#pragma once
#include <raylib.h>
#include <queue>

struct __DebugLine
{
    Vector2 start;
    Vector2 end;
    Color color;
};

std::queue<__DebugLine> g_DebugLines;

/// @brief Call every frame
/// @param color [OPTIONAL] Default `RED`
/// @param scale [OPTIONAL] Default `1.0f`
inline void __DebugDrawVector(const Vector2 &origin, const Vector2 &vector, Color color = RED, float scale = 1.0f)
{
    // using queue because we process then delete
    __DebugLine line;
    line.start = origin;
    line.end = {origin.x + vector.x * scale, origin.y + vector.y * scale};
    line.color = color;
    g_DebugLines.push(line);
}

inline void __ProcessDebugDraws()
{
    // draw lines of each frame, the next frame clears the previous line
    //
    while (!g_DebugLines.empty())
    {
        __DebugLine line = g_DebugLines.front();
        DrawLine(static_cast<int>(line.start.x), static_cast<int>(line.start.y),
                 static_cast<int>(line.end.x), static_cast<int>(line.end.y),
                 line.color);
        g_DebugLines.pop();
    }
}