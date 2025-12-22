#include "TrafficLight.hpp"

#include <iostream>
#include <limits>
#include <cmath>

#include "Car.hpp"
#include "VectorMath.hpp"
#include "MainScreen.hpp"
#include "Debug.hpp"

void UpdateTrafficLights(GameState &state)
{
    TrafficLightGroup &group = state.trafficLightGroup;

    // choose switching method
    if (TRAFFIC_LIGHT_ADAPTIVE_ENABLED)
    {
        SwitchTrafficLightsAdaptive(group); // placeholder
    }
    else
    {
        SwitchTrafficLightsTimed(group); // timed handles phase duration internally
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
    if (DEBUG_TRAFFIC_LIGHT_CAR_PASSED)
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
void SwitchTrafficLightsTimed(TrafficLightGroup &group)
{
    double now = GetTime();

    // compute elapsed time since phase started
    double elapsed = now - group.phaseStartTime;
    float phaseDuration = 0.f;

    switch (group.phase)
    {
    case TrafficLightTimedGroupPhase::GREEN_PHASE:
        phaseDuration = TRAFFIC_LIGHT_GREEN_DURATION;
        break;
    case TrafficLightTimedGroupPhase::YELLOW_PHASE:
        phaseDuration = TRAFFIC_LIGHT_YELLOW_DURATION;
        break;
    case TrafficLightTimedGroupPhase::ALL_RED_PHASE:
        phaseDuration = TRAFFIC_LIGHT_ALL_RED_DURATION;
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

void SwitchTrafficLightsAdaptive(TrafficLightGroup &group)
{
    const double now = GetTime();
    const double interval = 1.0 / TRAFFIC_LIGHT_ADAPTIVE_TICK_RATE;

    static int lastTick = -1;
    const int currentTick = static_cast<int>(now / interval);
    if (currentTick == lastTick)
        return;
    lastTick = currentTick;

    // compute waiting counts and find the heaviest queue
    size_t maxWaiting = 0;
    int maxIdx = -1;
    for (int i = 0; i < static_cast<int>(group.trafficLights.size()); ++i)
    {
        size_t wc = group.trafficLights[i].carsWaiting.size();
        if (wc > maxWaiting)
        {
            maxWaiting = wc;
            maxIdx = i;
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
        if (light.state == TrafficLightState::GO && greenAge >= TRAFFIC_LIGHT_ADAPTIVE_MIN_GREEN_TIME)
        {
            light.state = TrafficLightState::STOP;
        }
    }

    // If a light is currently green, let it complete at least the minimum green time before switching away.
    int currentGreenIdx = -1;
    for (int i = 0; i < static_cast<int>(group.trafficLights.size()); ++i)
    {
        if (group.trafficLights[i].state == TrafficLightState::GO)
        {
            currentGreenIdx = i;
            break;
        }
    }
    if (currentGreenIdx != -1)
    {
        double greenAge = now - group.trafficLights[currentGreenIdx].lastGreenTime;
        if (greenAge < static_cast<double>(TRAFFIC_LIGHT_ADAPTIVE_MIN_GREEN_TIME))
            return; // hold current green
    }

    // Priority rule: if the busiest queue has >= 3 cars and that light hasn't been green for >= 10s, make it green
    if (maxIdx != -1)
    {
        TrafficLight &candidate = group.trafficLights[maxIdx];
        double sinceGreen = now - candidate.lastGreenTime;
        if (candidate.carsWaiting.size() >= 3 && sinceGreen >= 10.0)
        {
            // switch candidate to GO and others to STOP
            for (int i = 0; i < static_cast<int>(group.trafficLights.size()); ++i)
            {
                if (i == maxIdx)
                {
                    if (group.trafficLights[i].state != TrafficLightState::GO)
                    {
                        group.trafficLights[i].state = TrafficLightState::GO;
                        group.trafficLights[i].lastGreenTime = now;
                    }
                }
                else
                {
                    group.trafficLights[i].state = TrafficLightState::STOP;
                }
            }
            return;
        }
    }

    // Fallback behavior:
    // If there's no current green, promote the busiest queue to green (so traffic flows).
    if (currentGreenIdx == -1 && maxIdx != -1)
    {
        group.trafficLights[maxIdx].state = TrafficLightState::GO;
        group.trafficLights[maxIdx].lastGreenTime = now;
    }

    // Also ensure that any light with zero waiting cars does not remain green indefinitely.
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
        Color trafficLightColor = TRAFFIC_LIGHT_OFF_COLOR;
        if (trafficLight.state == TrafficLightState::GO)
            trafficLightColor = TRAFFIC_LIGHT_ON_COLOR;
        else if (trafficLight.state == TrafficLightState::WAIT)
            trafficLightColor = TRAFFIC_LIGHT_YELLOW_COLOR;
        else
            trafficLightColor = TRAFFIC_LIGHT_OFF_COLOR;

        DrawCircleV(trafficLight.position, TRAFFIC_LIGHT_RADIUS, trafficLightColor);
    }
}

void ForceUpdateTrafficLights(GameState &state)
{
    SwitchTrafficLightsTimed(state.trafficLightGroup);
}

// Behavior:
//  - If already past the light -> allow.
//  - If far away               -> allow.
//  - If moving toward the signal:
//      * GO            -> allow
//      * WAIT (yellow) -> allow only if car is too close to stop
//      * STOP          -> do not allow
//  - Otherwise allow.
bool TrafficLightUpdateCarState(TrafficLightGroup &trafficLightGroup, Car &car)
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
    if (shortestDistance > TRAFFIC_LIGHT_CAR_DETECTION_RANGE)
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
        if (DEBUG_TRAFFIC_LIGHT_CAR_CAN_PASS)
            __DebugDrawVectorAB(car.position, closestLight->position, 2, true, GREEN);
        return true;
    }
    else if (closestLight->state == TrafficLightState::STOP)
    {
        // allow pass if before waiting position
        bool carBeforeWaitingPos = Vector2AfterPoint(closestLight->waitingPosition, car.position, closestLight->direction);
        if (carBeforeWaitingPos)
        {
            if (DEBUG_TRAFFIC_LIGHT_CAR_CAN_PASS)
                __DebugDrawVectorAB(car.position, closestLight->position, 2, true, RED);

            return true;
        }

        // else stop
        if (DEBUG_TRAFFIC_LIGHT_CAR_CAN_PASS)
            __DebugDrawVectorAB(car.position, closestLight->position, 2, true, Fade(RED, 0.5f));

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

        if (DEBUG_TRAFFIC_LIGHT_CAR_CAN_PASS)
            __DebugDrawVectorAB(car.position, closestLight->position, 2, true, ORANGE);

        // if too close, allow to move to clear intersection; otherwise stop
        return tooCloseToStop;
    }
}