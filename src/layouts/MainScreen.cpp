#include "MainScreen.hpp"

#include "WindowConfig.hpp"
#include "Roads.hpp"
#include "Debug.hpp"

void InitGameState(GameState &state)
{
    // traffic lights
    Vector2 topRoadLightPos = {
        WINDOW_CENTER.x,
        WINDOW_CENTER.y - ROAD_SIZE / 2 //
    };
    Vector2 leftRoadLightPos = {
        WINDOW_CENTER.x - ROAD_SIZE / 2,
        WINDOW_CENTER.y //
    };
    Vector2 bottomRoadLightPos = {
        WINDOW_CENTER.x,
        WINDOW_CENTER.y + ROAD_SIZE / 2 //
    };
    Vector2 rightRoadLightPos = {
        WINDOW_CENTER.x + ROAD_SIZE / 2,
        WINDOW_CENTER.y //
    };

    state.trafficLightGroup.trafficLights[0] = TrafficLight{topRoadLightPos, {0, 1}, true};
    state.trafficLightGroup.trafficLights[1] = TrafficLight{leftRoadLightPos, {1, 0}, false};
    state.trafficLightGroup.trafficLights[2] = TrafficLight{bottomRoadLightPos, {0, -1}, false};
    state.trafficLightGroup.trafficLights[3] = TrafficLight{rightRoadLightPos, {-1, 0}, false};
    state.trafficLightGroup.currentGroup = true;

    // cars
    constexpr int numCarsPerDirection = 5;
    constexpr float spacing = 100.f;
    Vector2 bottomCarStartPos = {
        WINDOW_CENTER.x + ROAD_SIZE / 4,
        WINDOW_SIZE.y + 100.f //
    };
    Vector2 topCarStartPos = {
        WINDOW_CENTER.x - ROAD_SIZE / 4,
        -100.f //
    };
    Vector2 leftCarStartPos = {
        -100.f,
        WINDOW_CENTER.y - ROAD_SIZE / 4 //
    };
    Vector2 rightCarStartPos = {
        WINDOW_SIZE.x + 100.f,
        WINDOW_CENTER.y + ROAD_SIZE / 4 //
    };

    auto addCarLambda = [&](const Vector2 &position, const Vector2 &velocity, const Vector2 &size)
    {
        Car newCar = {
            .position = position,
            .size = size,
            .desiredVelocity = velocity //
        };
        state.cars.push_back(newCar);
    };
    for (int i = 0; i < numCarsPerDirection; ++i)
    {
        // bottom to top
        addCarLambda(
            {bottomCarStartPos.x, bottomCarStartPos.y + i * spacing},
            {0.f, -CAR_SPEED},
            {CAR_WIDTH, CAR_HEIGHT} //
        );
        // top to bottom
        addCarLambda(
            {topCarStartPos.x, topCarStartPos.y - i * spacing},
            {0.f, CAR_SPEED},
            {CAR_WIDTH, CAR_HEIGHT} //
        );
        // left to right
        addCarLambda(
            {leftCarStartPos.x - i * spacing, leftCarStartPos.y},
            {CAR_SPEED, 0.f},
            {CAR_HEIGHT, CAR_WIDTH} //
        );
        // right to left
        addCarLambda(
            {rightCarStartPos.x + i * spacing, rightCarStartPos.y},
            {-CAR_SPEED, 0.f},
            {CAR_HEIGHT, CAR_WIDTH} //
        );
    }
}

void UpdateGameState(GameState &state)
{
    state.deltaTime = GetFrameTime();

    UpdateTrafficLights(state);

    for (auto &car : state.cars)
    {
        UpdateCar(car, state);
    }
}

void DrawMainScreen(const GameState &state)
{
    DrawRoads();
    DrawTrafficLightGroup(state.trafficLightGroup);

    for (const auto &car : state.cars)
    {
        DrawCar(car);
    }
}
