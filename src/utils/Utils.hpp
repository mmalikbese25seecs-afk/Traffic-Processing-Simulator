#pragma once
#include <raylib.h>
#include <cmath>

inline float ConvertDegreesToRadians(float degrees)
{
    return degrees * (M_PI / 180.0f);
}

inline float ConvertRadiansToDegrees(float radians)
{
    return radians * (180.0f / M_PI);
}

inline bool IsColorEqual(const Color &a, const Color &b)
{
    return (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a);
}

inline void DebugDrawVector(const Vector2 &origin, const Vector2 &vector, Color color = RED, float scale = 1.0f)
{
    // make sure drawn on top
    DrawLine(static_cast<int>(origin.x), static_cast<int>(origin.y),
             static_cast<int>(origin.x + vector.x * scale),
             static_cast<int>(origin.y + vector.y * scale),
             color);
}