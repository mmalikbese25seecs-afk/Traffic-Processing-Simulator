#pragma once
#include <raylib.h>
#include <queue>

struct DebugLine
{
    Vector2 start;
    Vector2 end;
    Color color;
};

// using queue because we process then delete
inline std::queue<DebugLine> g_DebugLines;
inline void DebugDrawVector(const Vector2 &origin, const Vector2 &vector, Color color = RED, float scale = 1.0f)
{
    DebugLine line;
    line.start = origin;
    line.end = {origin.x + vector.x * scale, origin.y + vector.y * scale};
    line.color = color;
    g_DebugLines.push(line);
}

inline void ProcessDebugDraws()
{
    while (!g_DebugLines.empty())
    {
        DebugLine line = g_DebugLines.front();
        DrawLine(static_cast<int>(line.start.x), static_cast<int>(line.start.y),
                 static_cast<int>(line.end.x), static_cast<int>(line.end.y),
                 line.color);
        g_DebugLines.pop();
    }
}