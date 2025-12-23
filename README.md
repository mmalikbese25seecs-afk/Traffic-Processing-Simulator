# Traffic Processing Simulator

This is a **traffic intersection simulator** built with C++ and Raylib, the purpose is to compare the traffic flow for timed and adaptive approach.

## Prerequisites

- Windows:
  - Visual Studio
  - x64 Command Lines Tools For Visual Studio
- MacOS:
  - `clang++` in CLIOptimizing tool selection...
- `ninja` in CLI
- `cmake` in CLI
- `git` in CLI
  
## How to Run

> On Windows use **x64 Command Lines Tools For Visual Studio**.

- Run `./setup` to setup the project *(installs fmt as example also)*
- use `./run` for running afterwards

## What It Does

1. Cars spawn from all four directions (north, south, east, west) at configurable rates
2. Four traffic lights control the intersection.
3. The algorithm supports two traffic light modes:
   - **Timed Mode**: Fixed green/yellow/red light durations with coordinated phases
   - **Adaptive Mode**: Dynamically adjusts light timing based on traffic density and wait times
4. Cars detect other vehicles in front of them and maintain safe distances
5. Each car operates with states (MOVING, WAITING) and responds to traffic lights and other vehicles
6. Live configuration editor accessible during runtime (press `C`)

## Core Components

**Car System**:

- Each car has position, velocity, size, state, and unique ID
- Cars detect obstacles in a configurable detection radius, with start and end angles, making a pie-slice detection area
- Cars stop automatically when approaching other vehicles or red lights

**Traffic Light System**:

- Each traffic light has position, state (GREEN, YELLOW, RED), and timers
- Traffic lights are combined into pairs for north-south and east-west directions, and stored in `TrafficLightGroup`
- Seperate functions for timed and adaptive light control algorithms
- Adaptive mode uses car counts and wait times to adjust light durations dynamically
- Traffic lights influence car behavior based on proximity and state

**Configuration System**:

- Tree-based configuration with type safety (int, float, bool, string)
- Runtime editing through in-game UI
- Persistent storage to config.txt
- Parameters include spawn rates, detection ranges, light timings, and debug options

**Rendering** (MainScreen.cpp):

Renders in the following order:

- Renders cars, traffic lights, and intersection layout
- UI overlay for configuration editing
- Debug visualization options for detection ranges, car states, and collision zones

## Key Features

- **Pause/Resume** (press `P`)
- **Restart** simulation (press `R`)
- **Live configuration editing** (press `C` to toggle UI)
- **Save/Load** configurations (press `S`/`L` in config mode)
- **Debug modes** for visualizing:
  - Car detection arcs
  - Traffic light waiting positions
  - Car states
  - Collision detection zones

## Technical Architecture

- **Language**: C++
- **Graphics**: Raylib for rendering and input
- **Build System**: CMake with vcpkg for dependency management
- **Platform Support**: Cross-platform (Windows, macOS, *Linux)

> - Platform support for Linux is untested but should work with minor adjustments.

## Configuration Parameters

### GUI Structure

- **Main Configuration** : Core simulation parameters

  - **Car** : Car-related settings

    - **Spawn Chance** : Spawn chance of a car
    - **Collision** : Collision detection parameters for cars

      - **Radius** : Collision detection radius for cars
      - **Angle (deg)** : Detection angle in degrees for cars
      - **Detection Range** : Detection range for cars

  - **Traffic Light** : Traffic light behavior configuration

    - **Detection** : Vehicle detection parameters for traffic lights
      - **Car Detection Range** : Traffic light car detection range

    - **Timing** : Fixed timing configuration

      - **Green Duration** : Green light duration in seconds
      - **Yellow Duration** : Yellow light duration in seconds
      - **All Red Duration** : All-red light duration in seconds

    - **Adaptive** : Adaptive traffic light mode
      - **Enabled** : Enable adaptive traffic light mode
      - **Refresh Interval** : Refresh interval for adaptive mode in seconds
      - **Min Green Time** : Minimum green light duration in adaptive mode in seconds

- **Debug Options** : Debug visualization and diagnostics

  - **Car** : Car debug visualization
    - **Collision Detection Arc** : Whether to draw car detection regions
    - **Detection Other Cars** : Whether to draw a line between detected cars
    - **State (Waiting/ Moving)** : Whether to draw car states above cars (WAITING: orange, MOVING: green)

  - **Traffic Light** : Traffic light debug visualization

    - **Detection Range** : Whether to draw a line between traffic light and detected cars
    - **Stats** : Whether to show statistics of how many cars have passed and how many are waiting per traffic light
    - **Traffic Light Wait Positions** : Whether to draw traffic light waiting positions

### Settings in `config.txt`

| ID                                         | Description                                                             |
| ------------------------------------------ | ----------------------------------------------------------------------- |
| `car_spawn_chance`                         | Spawn chance of a car                                                   |
| `car_collision_radius`                     | Collision Detection Radius for cars                                     |
| `car_collision_angle_deg`                  | Detection Angle in degrees for cars                                     |
| `car_collision_detection_range`            | Detection Range for cars                                                |
| `traffic_light_car_detection_range`        | Traffic Light Car Detection Range                                       |
| `traffic_light_green_duration`             | Green Light Duration (seconds)                                          |
| `traffic_light_yellow_duration`            | Yellow Light Duration (seconds)                                         |
| `traffic_light_all_red_duration`           | All Red Light Duration (seconds)                                        |
| `traffic_light_adaptive_enabled`           | Enable Adaptive Traffic Light Mode                                      |
| `traffic_light_adaptive_refresh_interval`  | Refresh interval for Adaptive Mode (seconds)                            |
| `traffic_light_adaptive_min_green_time`    | Minimum Green Light Duration in Adaptive Mode (seconds)                 |
| `debug_car_show_collision_arc`             | Whether to draw car detection regions                                   |
| `debug_car_show_other_car_detection`       | Whether to draw a line between detected cars                            |
| `debug_car_show_state`                     | Whether to draw car states above cars (WAITING: orange, MOVING: green)  |
| `debug_traffic_light_show_detection_range` | Whether to draw a line between traffic light and detected cars          |
| `debug_traffic_light_show_stats`           | Whether to show statistics of cars passed and waiting per traffic light |
| `debug_traffic_light_show_wait_positions`  | Whether to draw traffic light waiting positions                         |
