#include "TrafficLight.hpp"

#include <iostream>
#include <limits>

#include "Car.hpp"
#include "VectorMath.hpp"
#include "MainScreen.hpp"

void UpdateTrafficLights(GameState &state)
{
    const double time = GetTime();
    const int interval = TRAFFIC_LIGHT_SWITCH_INTERVAL; // seconds

    static int lastTick = -1;
    const int currentTick = static_cast<int>(time) / interval;

    if (currentTick == lastTick)
        return;

    // code below runs once per interval
    SwitchTrafficLights(state.trafficLightGroup);

    lastTick = currentTick;
}

void SwitchTrafficLights(TrafficLightGroup &group)
{
    if (group.currentGroup)
    {
        // turn off 0, 2; turn on 1, 3
        group.trafficLights[0].isOn = false;
        group.trafficLights[1].isOn = true;
        group.trafficLights[2].isOn = false;
        group.trafficLights[3].isOn = true;
    }
    else
    {
        // turn on 0, 2; turn off 1, 3
        group.trafficLights[0].isOn = true;
        group.trafficLights[1].isOn = false;
        group.trafficLights[2].isOn = true;
        group.trafficLights[3].isOn = false;
    }

    // toggle state
    group.currentGroup = !group.currentGroup;
}

void DrawTrafficLightGroup(const TrafficLightGroup &light)
{
    for (const auto &trafficLight : light.trafficLights)
    {
        // draw triangle for direction
        // normalize direction
        Vector2 forward = trafficLight.direction;
        Vector2Normalize(forward);

        // perpendicular direction
        Vector2 right = {forward.y, -forward.x};
        Vector2 left = {-right.x, -right.y};

        // triangle base points (on circle edge)
        Vector2 leftPoint = {
            trafficLight.position.x + left.x * (TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING),
            trafficLight.position.y + left.y * (TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING) //
        };
        Vector2 topPoint = {
            trafficLight.position.x + forward.x * (TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING + TRAFFIC_LIGHT_CAP_DISTANCE),
            trafficLight.position.y + forward.y * (TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING + TRAFFIC_LIGHT_CAP_DISTANCE) //
        };
        Vector2 rightPoint = {
            trafficLight.position.x + right.x * (TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING),
            trafficLight.position.y + right.y * (TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING) //
        };

        DrawTriangle(leftPoint, topPoint, rightPoint, TRAFFIC_LIGHT_BG_COLOR);
        // draw outline circle
        DrawCircleV(trafficLight.position, TRAFFIC_LIGHT_RADIUS + TRAFFIC_LIGHT_BG_PADDING, TRAFFIC_LIGHT_BG_COLOR);
        // draw colored circle
        Color trafficLightColor = trafficLight.isOn ? TRAFFIC_LIGHT_ON_COLOR : TRAFFIC_LIGHT_OFF_COLOR;
        DrawCircleV(trafficLight.position, TRAFFIC_LIGHT_RADIUS, trafficLightColor);
    }
}

bool CanCarPass(const TrafficLightGroup &trafficLight, const Car &car)
{
    // get shortest distance from traffic light to car
    TrafficLight closestLight;
    float shortestDistance = std::numeric_limits<float>::max();
    for (auto trafficLight : trafficLight.trafficLights)
    {
        float distance = Vector2Distance(trafficLight.position, car.position);
        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            closestLight = trafficLight;
        }
    }

    if (shortestDistance > TRAFFIC_LIGHT_STOP_DISTANCE)
        return true; // far away from traffic light

    // check if car is moving towards signal
    bool movingTowardsSignal = Vector2Aligned(car.desiredVelocity, closestLight.direction);

    if (movingTowardsSignal && !closestLight.isOn)
        return false;

    return true;
}
