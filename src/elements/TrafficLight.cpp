#include "TrafficLight.hpp"
#include "Utils.hpp"

void UpdateTrafficLight(TrafficLight &trafficLight)
{
    // change color every second
    static float timer = 0.0f;
    timer += GetFrameTime();
    if (timer >= 1.0f)
    {
        // red -> orange
        if (IsColorEqual(trafficLight.currentColor, RED))
        {
            trafficLight.currentColor = ORANGE;
            trafficLight.currentState = TrafficLightState::TORANGE;
        }
        // orange -> green
        else if (IsColorEqual(trafficLight.currentColor, ORANGE))
        {
            trafficLight.currentColor = GREEN;
            trafficLight.currentState = TrafficLightState::TGREEN;
        }
        // green -> red
        else
        {
            trafficLight.currentColor = RED;
            trafficLight.currentState = TrafficLightState::TRED;
        }
        timer = 0.0f;
    }
}

void DrawTrafficLight(const TrafficLight &trafficLight)
{
    // Draw the traffic light box
    DrawRectangleRec({trafficLight.position.x, trafficLight.position.y, trafficLight.size.x, trafficLight.size.y}, trafficLight.currentColor);
}
