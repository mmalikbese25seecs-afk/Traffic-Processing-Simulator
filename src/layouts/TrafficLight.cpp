#include "TrafficLight.hpp"

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

void DrawTrafficLight(Vector2 center, float size, Color color)
{
    DrawCircleV(center, size, color);
}
