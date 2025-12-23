#include "config.hpp"
#include "ConfigTree.hpp"

Node BuildConfigTree()
{
    Node root;
    root.title = "CONFIG";

    // Main Configuration
    Node main_config;
    main_config.title = "Main Configuration";

    // Car Config
    Node car;
    car.title = "Car";

    car.children.push_back(Node{
        .title = "Spawn Chance",
        .value = ConfigValue{"main_screen_spawn_chance", ValueType::Float, 1.0f, 0.0f, 1.0f},
        .children = {},
        .collapsed = false //
    });

    Node carCollision;
    carCollision.title = "Collision";

    carCollision.children.push_back(Node{
        .title = "Radius",
        .value = ConfigValue{"radius", ValueType::Int, 50, 0, 200},
        .children = {},
        .collapsed = false //
    });
    carCollision.children.push_back(Node{
        .title = "Angle (deg)",
        .value = ConfigValue{"angle_deg", ValueType::Int, 45, 0, 180},
        .children = {},
        .collapsed = false //
    });
    carCollision.children.push_back(Node{
        .title = "Detection Range",
        .value = ConfigValue{"detection_range", ValueType::Int, 100, 0, 500},
        .children = {},
        .collapsed = false //
    });

    car.children.push_back(carCollision);
    main_config.children.push_back(car);

    // Traffic Light Config
    Node tl;
    tl.title = "Traffic Light";

    Node tl_det;
    tl_det.title = "Detection";
    tl_det.children.push_back(Node{
        .title = "Car Detection Range",
        .value = ConfigValue{"tl_car_detection_range", ValueType::Int, 50, 0, 200},
        .children = {},
        .collapsed = false //
    });
    tl.children.push_back(tl_det);

    Node tl_timing;
    tl_timing.title = "Timing";
    tl_timing.children.push_back(Node{
        .title = "Green Duration",
        .value = ConfigValue{"green_duration", ValueType::Int, 10, 1, 60},
        .children = {},
        .collapsed = false //
    });
    tl_timing.children.push_back(Node{
        .title = "Yellow Duration",
        .value = ConfigValue{"yellow_duration", ValueType::Int, 3, 1, 10},
        .children = {},
        .collapsed = false //
    });
    tl_timing.children.push_back(Node{
        .title = "All Red Duration",
        .value = ConfigValue{"all_red_duration", ValueType::Float, 0.5f, 0.0f, 5.0f},
        {},
        false});
    tl.children.push_back(tl_timing);

    Node tl_adaptive;
    tl_adaptive.title = "Adaptive";
    tl_adaptive.children.push_back(Node{
        .title = "Enabled",
        .value = ConfigValue{"tl_adaptive_enabled", ValueType::Bool, true},
        .children = {},
        .collapsed = false //
    });
    tl_adaptive.children.push_back(Node{
        .title = "Refresh Interval",
        .value = ConfigValue{"tl_adaptive_tick_rate", ValueType::Int, 1, 1, 10},
        .children = {},
        .collapsed = false //
    });
    tl_adaptive.children.push_back(Node{
        .title = "Min Green Time",
        .value = ConfigValue{"tl_adaptive_min_green_time", ValueType::Int, 5, 1, 20},
        .children = {},
        .collapsed = false //
    });
    main_config.children.push_back(tl);

    // Debug Options
    Node debug_options;
    debug_options.title = "Debug Options";

    Node debug_car;
    debug_car.title = "Car";
    debug_car.children.push_back(Node{
        .title = "Collision Detection Arc",
        .value = ConfigValue{"debug_car_detection_arc", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_car.children.push_back(Node{
        .title = "Detection Other Cars",
        .value = ConfigValue{"debug_car_detection_other_cars", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_car.children.push_back(Node{
        .title = "State (Waiting/ Moving)",
        .value = ConfigValue{"debug_car_state", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_options.children.push_back(debug_car);

    Node debug_tl;
    debug_tl.title = "Traffic Light";
    debug_tl.children.push_back(Node{
        .title = "Detection Range",
        .value = ConfigValue{"debug_tl_car_can_pass", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_tl.children.push_back(Node{
        .title = "Stats",
        .value = ConfigValue{"debug_tl_car_passed", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_tl.children.push_back(Node{
        .title = "Traffic Light Wait Positions",
        .value = ConfigValue{"main_screen_debug_draw_traffic_light_positions", ValueType::Bool, true},
        .children = {},
        .collapsed = false //
    });
    debug_options.children.push_back(debug_tl);

    root.children.push_back(main_config);
    root.children.push_back(debug_options);

    return root;
}
