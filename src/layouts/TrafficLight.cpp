#include "TrafficLight.hpp"

#include <iostream>
#include <limits>
#include <cmath>
#include <string>

#include "Car.hpp"
#include "VectorMath.hpp"
#include "MainScreen.hpp"
#include "Debug.hpp"

void UpdateTrafficLights(GameState &state)
{
    TrafficLightGroup &group = state.trafficLightGroup;

    // choose switching method
    if (GetConfigBool(state.rootConfigNode, "tl_adaptive_enabled", false))
    {
        SwitchTrafficLightsAdaptive(group, state.rootConfigNode);
    }
    else
    {
        SwitchTrafficLightsTimed(group, state.rootConfigNode);
    }

    // update carsWaiting sets for debugging
    for (auto &light : group.trafficLights)
    {
        for (const Car &car : state.cars)
        {
            if (car.state == CarState::WAITING)
            {
                if (Vector2Aligned(car.desiredVelocity, light.direction))
                    light.carsWaiting.insert(car.id);
            }
        }
    }

    // debug display of cars passed / waiting
    if (GetConfigBool(state.rootConfigNode, "debug_tl_car_passed", false))
    {
        for (auto &light : group.trafficLights)
        {
            int passedCount = static_cast<int>(light.carsPassed.size());
            int waitingCount = static_cast<int>(light.carsWaiting.size());

            __DebugDrawText({light.position.x, light.position.y + 20}, "P: " + std::to_string(passedCount), 16, true, GREEN);
            __DebugDrawText({light.position.x, light.position.y + 40}, "W: " + std::to_string(waitingCount), 16, true, ORANGE);
        }
    }
}

// Advances the traffic light group's phase machine:
// GREEN_PHASE -> YELLOW_PHASE (current group's lights become WAIT)
// YELLOW_PHASE -> ALL_RED_PHASE (both groups STOP)
// ALL_RED_PHASE -> GREEN_PHASE (toggle currentGroup; that group's lights become GO)
void SwitchTrafficLightsTimed(TrafficLightGroup &group, const Node &configNode)
{
    double now = GetTime();

    // compute elapsed time since phase started
    double elapsed = now - group.phaseStartTime;
    float phaseDuration = 0.f;

    switch (group.phase)
    {
    case TrafficLightTimedGroupPhase::GREEN_PHASE:
        phaseDuration = GetConfigInt(configNode, "green_duration", 10);
        break;
    case TrafficLightTimedGroupPhase::YELLOW_PHASE:
        phaseDuration = GetConfigInt(configNode, "yellow_duration", 3);
        break;
    case TrafficLightTimedGroupPhase::ALL_RED_PHASE:
        phaseDuration = GetConfigFloat(configNode, "all_red_duration", 0.5f);
        break;
    }

    // only switch phase if duration has passed
    if (elapsed < phaseDuration)
        return;

    // advance phase
    switch (group.phase)
    {
    case TrafficLightTimedGroupPhase::GREEN_PHASE:
    {
        for (size_t i = 0; i < group.trafficLights.size(); ++i)
        {
            bool isCurrentPair = (group.currentGroup && (i == 0 || i == 2)) || (!group.currentGroup && (i == 1 || i == 3));
            group.trafficLights[i].state = isCurrentPair ? TrafficLightState::WAIT : TrafficLightState::STOP;
        }
        group.phase = TrafficLightTimedGroupPhase::YELLOW_PHASE;
        break;
    }
    case TrafficLightTimedGroupPhase::YELLOW_PHASE:
    {
        for (auto &light : group.trafficLights)
            light.state = TrafficLightState::STOP;
        group.phase = TrafficLightTimedGroupPhase::ALL_RED_PHASE;
        break;
    }
    case TrafficLightTimedGroupPhase::ALL_RED_PHASE:
    {
        group.currentGroup = !group.currentGroup;
        for (size_t i = 0; i < group.trafficLights.size(); ++i)
        {
            bool isCurrentPair = (group.currentGroup && (i == 0 || i == 2)) || (!group.currentGroup && (i == 1 || i == 3));
            group.trafficLights[i].state = isCurrentPair ? TrafficLightState::GO : TrafficLightState::STOP;
        }
        group.phase = TrafficLightTimedGroupPhase::GREEN_PHASE;
        break;
    }
    }

    // mark new phase start
    group.phaseStartTime = now;
}

void SwitchTrafficLightsAdaptive(TrafficLightGroup &group, const Node &configNode)
{
    const double now = GetTime();
    const double interval = 1.0 / GetConfigInt(configNode, "tl_adaptive_tick_rate", 1);

    static int lastTick = -1;
    const int currentTick = static_cast<int>(now / interval);
    if (currentTick == lastTick)
        return;
    lastTick = currentTick;

    // compute waiting counts and find the heaviest queue
    size_t maxWaiting = 0;
    TrafficLight *biggestQueueLight = nullptr;
    for (TrafficLight &light : group.trafficLights)
    {
        size_t waiting = light.carsWaiting.size();
        if (waiting > maxWaiting)
        {
            maxWaiting = waiting;
            biggestQueueLight = &light;
        }
    }

    // if nobody waiting return
    if (maxWaiting == 0)
        return;

    // first, turn off any green lights with zero waiting cars
    for (auto &light : group.trafficLights)
    {
        if (light.carsWaiting.empty() && light.state == TrafficLightState::GO)
        {
            light.state = TrafficLightState::STOP;
        }
    }
    // next, turn off any green lights that have exceeded minimum green time
    for (auto &light : group.trafficLights)
    {
        double greenAge = now - light.lastGreenTime;
        if (light.state == TrafficLightState::GO && greenAge >= GetConfigInt(configNode, "tl_adaptive_min_green_time", 5))
        {
            light.state = TrafficLightState::STOP;
        }
    }
    // If a light is currently green, let it complete at least the minimum green time before switching away.
    for (auto &light : group.trafficLights)
    {
        if (light.state == TrafficLightState::GO)
        {
            double greenAge = now - light.lastGreenTime;
            if (greenAge < GetConfigInt(configNode, "tl_adaptive_min_green_time", 5))
            {
                return; // do not switch yet
            }
        }
    }

    // Priority rule: if the busiest queue has >= 3 cars and that light hasn't been green for >= 10s, make it green
    if (!biggestQueueLight)
        return;

    double sinceGreen = now - biggestQueueLight->lastGreenTime;
    if (biggestQueueLight->carsWaiting.size() >= 3 && sinceGreen >= 10.0)
    {
        // switch biggest to GO and others to STOP
        biggestQueueLight->state = TrafficLightState::GO;
        biggestQueueLight->lastGreenTime = now;
        for (auto &light : group.trafficLights)
        {
            if (&light != biggestQueueLight)
            {
                light.state = TrafficLightState::STOP;
            }
        }
        return;
    }

    // Fallback behavior:
    // If there's no green, make the busiest queue to green
    bool anyGreen = false;
    for (const auto &light : group.trafficLights)
    {
        if (light.state == TrafficLightState::GO)
        {
            anyGreen = true;
            break;
        }
    }
    if (!anyGreen && biggestQueueLight)
    {
        biggestQueueLight->state = TrafficLightState::GO;
        biggestQueueLight->lastGreenTime = now;
    }
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

        // draw colored circle based on state
        Color trafficLightColor = RED;
        if (trafficLight.state == TrafficLightState::GO)
            trafficLightColor = GREEN;
        else if (trafficLight.state == TrafficLightState::WAIT)
            trafficLightColor = YELLOW;
        else
            trafficLightColor = RED;

        DrawCircleV(trafficLight.position, TRAFFIC_LIGHT_RADIUS, trafficLightColor);
    }
}

void ForceUpdateTrafficLights(GameState &state)
{
    if (GetConfigBool(state.rootConfigNode, "tl_adaptive_enabled", false))
    {
        SwitchTrafficLightsAdaptive(state.trafficLightGroup, state.rootConfigNode);
    }
    else
    {
        SwitchTrafficLightsTimed(state.trafficLightGroup, state.rootConfigNode);
    }
}

// Behavior:
//  - If already past the light -> allow.
//  - If far away               -> allow.
//  - If moving toward the signal:
//      * GO            -> allow
//      * WAIT (yellow) -> allow only if car is too close to stop
//      * STOP          -> do not allow
//  - Otherwise allow.
bool TrafficLightUpdateCarState(TrafficLightGroup &trafficLightGroup, Car &car, const GameState &state)
{
    // get shortest distance from traffic light to car
    TrafficLight *closestLight = nullptr;
    float shortestDistance = std::numeric_limits<float>::max();

    for (TrafficLight &light : trafficLightGroup.trafficLights)
    {
        float distance = Vector2Distance(light.position, car.position);
        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            closestLight = &light;
        }
    }

    if (!closestLight)
        return true;

    // far away from traffic light -> ignore it until close enough
    if (shortestDistance > GetConfigInt(state.rootConfigNode, "tl_car_detection_range", 50))
        return true;

    // check if car is moving towards the signal
    bool movingTowardsSignal = Vector2Aligned(car.desiredVelocity, closestLight->direction);
    if (!movingTowardsSignal)
        return true;

    // check if car has passed the traffic light
    bool hasPassedTrafficLight = Vector2AfterPoint(car.position, closestLight->position, closestLight->direction);
    if (hasPassedTrafficLight) // is moving towards but already passed
    {
        // mark that car has passed this light
        closestLight->carsPassed.insert(car.id);
        // remove from waiting set if was there
        closestLight->carsWaiting.erase(car.id);
        car.state = CarState::MOVING;
        return true;
    }
    // moving towards and not yet passed; and light is red
    else if (!hasPassedTrafficLight && closestLight->state == TrafficLightState::STOP)
    {
        // mark that car is waiting at this light
        closestLight->carsWaiting.insert(car.id);
        car.state = CarState::WAITING;
    }

    // determine behavior based on light state
    if (closestLight->state == TrafficLightState::GO)
    {
        if (GetConfigBool(state.rootConfigNode, "debug_tl_car_can_pass", false))
        {
            __DebugDrawVectorAB(car.position, closestLight->position, 2, true, GREEN);
            float range = GetConfigInt(state.rootConfigNode, "tl_car_detection_range", 50);
            __DebugDrawCircleArc(closestLight->position, range, 0.f, 360.f, Fade(GREEN, 0.1f));
        }
        return true;
    }
    else if (closestLight->state == TrafficLightState::STOP)
    {
        // allow pass if before waiting position
        bool carBeforeWaitingPos = Vector2AfterPoint(closestLight->waitingPosition, car.position, closestLight->direction);
        if (carBeforeWaitingPos)
        {
            if (GetConfigBool(state.rootConfigNode, "debug_tl_car_can_pass", false))
            {
                __DebugDrawVectorAB(car.position, closestLight->position, 2, true, RED);
                float range = GetConfigInt(state.rootConfigNode, "tl_car_detection_range", 50);
                __DebugDrawCircleArc(closestLight->position, range, 0.f, 360.f, Fade(RED, 0.1f));
            }

            return true;
        }

        // else stop
        if (GetConfigBool(state.rootConfigNode, "debug_tl_car_can_pass", false))
        {
            __DebugDrawVectorAB(car.position, closestLight->position, 2, true, Fade(RED, 0.5f));
            float range = GetConfigInt(state.rootConfigNode, "tl_car_detection_range", 50);
            __DebugDrawCircleArc(closestLight->position, range, 0.f, 360.f, Fade(RED, 0.1f));
        }

        return false;
    }
    else // WAIT (yellow)
    {
        // approximate car speed (length of desired velocity)
        float speed = Vector2Length(car.desiredVelocity);

        // approximate minimum distance needed to stop in time.
        // distance = speed * reaction_time + buffer
        float minDistanceToStop = speed * 1.5f + 5.f;

        bool tooCloseToStop = shortestDistance <= minDistanceToStop;

        if (GetConfigBool(state.rootConfigNode, "debug_tl_car_can_pass", false))
        {
            __DebugDrawVectorAB(car.position, closestLight->position, 2, true, ORANGE);
            float range = GetConfigInt(state.rootConfigNode, "tl_car_detection_range", 50);
            __DebugDrawCircleArc(closestLight->position, range, 0.f, 360.f, Fade(ORANGE, 0.1f));
        }

        // if too close, allow to move to clear intersection; otherwise stop
        return tooCloseToStop;
    }
}