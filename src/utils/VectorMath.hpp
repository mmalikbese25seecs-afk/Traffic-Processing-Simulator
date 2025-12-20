#pragma once
#include <raylib.h>

#include <cmath>

inline void WrapAngle(float &angle, bool degrees)
{
    float fullCircle = degrees ? 360.f : 2.f * M_PI;
    angle = fmodf(angle, fullCircle); // fmodf = % for floats
    if (angle < 0)
        angle += fullCircle;
}

inline float AngleToRadians(float degrees)
{
    WrapAngle(degrees, true);
    return degrees * (M_PI / 180.f);
}

inline float AngleToDegrees(float radians)
{
    WrapAngle(radians, false);
    return radians * (180.f / M_PI);
}

inline float Vector2Dot(const Vector2 &v1, const Vector2 &v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}

inline bool Vector2IsZero(const Vector2 &v)
{
    return (v.x == 0.f) && (v.y == 0.f);
}

inline float Vector2Length(const Vector2 &v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

inline float Vector2Distance(const Vector2 &v1, const Vector2 &v2)
{
    return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

inline void Vector2Normalize(Vector2 &v)
{
    float len = Vector2Length(v);
    if (len > 0.0001f)
    {
        v.x /= len;
        v.y /= len;
    }
    else
    {
        v.x = 0.f;
        v.y = 0.f;
    }
}

inline bool Vector2Aligned(const Vector2 &v1, const Vector2 &v2, float toleranceAngleDegrees = 10.f)
{
    if (Vector2IsZero(v1) || Vector2IsZero(v2))
        return false;

    // using atan2 to get angles
    float angle1 = atan2f(v1.y, v1.x);
    float angle2 = atan2f(v2.y, v2.x);

    float angleDiff = angle1 - angle2;
    // wrap to [-pi, pi]
    angleDiff = fmodf(angleDiff + M_PI, 2.f * M_PI) - M_PI;
    // absolute value; so works for both sides
    angleDiff = fabsf(angleDiff);

    return angleDiff <= AngleToRadians(toleranceAngleDegrees);
}
