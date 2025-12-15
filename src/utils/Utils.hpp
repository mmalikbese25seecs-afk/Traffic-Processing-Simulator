#pragma once
#include <raylib.h>
#include <cmath>
#include <concepts>
#include <vector>

template <typename T>
    requires std::totally_ordered<T>
inline void Clamp(T &value, T min, T max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
}

template <typename T>
    requires std::totally_ordered<T>
inline void Clamp01(T &value)
{
    Clamp(value, static_cast<T>(0), static_cast<T>(1));
}

template <typename T>
    requires std::is_floating_point_v<T>
inline void WrapAngle360(T &angle)
{
    angle = std::fmod(angle, static_cast<T>(360));
    if (angle < 0)
        angle += static_cast<T>(360);
}

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
