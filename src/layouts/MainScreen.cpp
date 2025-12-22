#include "MainScreen.hpp"

#include "WindowConfig.hpp"
#include "Roads.hpp"
#include "Debug.hpp"
#include "RandomGen.hpp"

void m_spawnCars(GameState &state)
{
    Vector2 bottomCarStartPos = {
        WINDOW_CENTER.x - ROAD_SIZE / 4,
        WINDOW_SIZE.y + START_OFFSET //
    };
    Vector2 topCarStartPos = {
        WINDOW_CENTER.x + ROAD_SIZE / 4,
        -START_OFFSET //
    };
    Vector2 leftCarStartPos = {
        -START_OFFSET,
        WINDOW_CENTER.y - ROAD_SIZE / 4 //
    };
    Vector2 rightCarStartPos = {
        WINDOW_SIZE.x + START_OFFSET,
        WINDOW_CENTER.y + ROAD_SIZE / 4 //
    };

    auto addCarLambda = [&](const Vector2 &position, const Vector2 &velocity, const Vector2 &size)
    {
        // skip based on random chance
        if (!GetRandomChance(MAIN_SCREEN_CAR_SKIP_SPAWN_CHANCE))
            return;

        Car newCar = {
            .id = state._lastAddedCarId,
            .position = position,
            .size = size,
            .desiredVelocity = velocity //
        };
        state.cars.push_back(newCar);

        state._lastAddedCarId++;
    };
    // bottom to top
    addCarLambda(
        {bottomCarStartPos.x, bottomCarStartPos.y + SPACING},
        {0.f, -CAR_SPEED},
        {CAR_WIDTH, CAR_HEIGHT} //
    );
    // top to bottom
    addCarLambda(
        {topCarStartPos.x, topCarStartPos.y - SPACING},
        {0.f, CAR_SPEED},
        {CAR_WIDTH, CAR_HEIGHT} //
    );
    // left to right
    addCarLambda(
        {leftCarStartPos.x - SPACING, leftCarStartPos.y},
        {CAR_SPEED, 0.f},
        {CAR_WIDTH, CAR_HEIGHT} //
    );
    // right to left
    addCarLambda(
        {rightCarStartPos.x + SPACING, rightCarStartPos.y},
        {-CAR_SPEED, 0.f},
        {CAR_WIDTH, CAR_HEIGHT} //
    );
}

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

    state.trafficLightGroup.trafficLights[0] = TrafficLight{
        topRoadLightPos,
        {0, 1},
        {
            topRoadLightPos.x + ROAD_SIZE / 4,
            topRoadLightPos.y - TRAFFIC_LIGHT_WAIT_POS_OFFSET //
        },
        TrafficLightState::GO //
    };
    state.trafficLightGroup.trafficLights[1] = TrafficLight{
        leftRoadLightPos,
        {1, 0},
        {
            leftRoadLightPos.x - TRAFFIC_LIGHT_WAIT_POS_OFFSET,
            leftRoadLightPos.y - ROAD_SIZE / 4 //
        },
        TrafficLightState::STOP //
    };
    state.trafficLightGroup.trafficLights[2] = TrafficLight{
        bottomRoadLightPos,
        {0, -1},
        {
            bottomRoadLightPos.x - ROAD_SIZE / 4,
            bottomRoadLightPos.y + TRAFFIC_LIGHT_WAIT_POS_OFFSET //
        },
        TrafficLightState::GO //
    };
    state.trafficLightGroup.trafficLights[3] = TrafficLight{
        rightRoadLightPos,
        {-1, 0},
        {
            rightRoadLightPos.x + TRAFFIC_LIGHT_WAIT_POS_OFFSET,
            rightRoadLightPos.y + ROAD_SIZE / 4 //
        },
        TrafficLightState::STOP //
    };
    state.trafficLightGroup.currentGroup = true;
    ForceUpdateTrafficLights(state);
}

void m_cleanupCars(GameState &state)
{
    state.cars.erase(
        std::remove_if(
            state.cars.begin(),
            state.cars.end(),
            [](const Car &car)
            {
                return (car.position.x < -100.f || car.position.x > WINDOW_SIZE.x + 100.f ||
                        car.position.y < -100.f || car.position.y > WINDOW_SIZE.y + 100.f);
            }),
        state.cars.end());
}

void m_updateCarSpawning(GameState &state)
{
    const double time = GetTime();
    const double interval = 1.0 / state.carsPerSecond;

    static int lastTick = -1;
    const int currentTick = static_cast<int>(time / interval);

    if (currentTick == lastTick)
        return;

    // code below runs once per interval
    m_spawnCars(state);
    m_cleanupCars(state);

    lastTick = currentTick;
}

void UpdateGameState(GameState &state)
{
    if (state.paused)
        return;

    state.deltaTime = GetFrameTime();

    m_updateCarSpawning(state);
    UpdateTrafficLights(state);
    for (auto &car : state.cars)
    {
        UpdateCar(car, state);
    }
}

void RestartGameState(GameState &state)
{
    // clear existing state
    state.trafficLightGroup = TrafficLightGroup{};
    ForceUpdateTrafficLights(state);
    state.cars.clear();
    InitGameState(state);
}

void PauseGameState(GameState &state)
{
    state.paused = true;
}

void ResumeGameState(GameState &state)
{
    state.paused = false;
}

void m_debugDrawGameState(const GameState &state)
{
    if (MAIN_SCREEN_DEBUG_DRAW_TRAFFIC_LIGHT_POSITIONS)
    {
        for (const auto &light : state.trafficLightGroup.trafficLights)
        {
            __DebugDrawPoint(light.waitingPosition, 8.f, MAGENTA);
        }
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

    m_debugDrawGameState(state);
}
