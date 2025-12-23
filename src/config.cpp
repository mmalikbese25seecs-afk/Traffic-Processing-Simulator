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

    car.children.push_back(
        Node{"Spawn Chance",
             ConfigValue{"main_screen_spawn_chance", ValueType::Float, 1.0f, 0.0f, 1.0f},
             {},
             false});

    Node carCollision;
    carCollision.title = "Collision";

    carCollision.children.push_back(
        Node{"Radius", ConfigValue{"radius", ValueType::Int, 50, 0, 200}, {}, false});
    carCollision.children.push_back(
        Node{"Angle(deg)", ConfigValue{"angle_deg", ValueType::Int, 45, 0, 180}, {}, false});
    carCollision.children.push_back(
        Node{"Detection Range", ConfigValue{"detection_range", ValueType::Int, 100, 0, 500}, {}, false});

    car.children.push_back(carCollision);
    main_config.children.push_back(car);

    // Traffic Light Config
    Node tl;
    tl.title = "Traffic Light";

    Node tl_det;
    tl_det.title = "Detection";
    tl_det.children.push_back(
        Node{"Car Detection Range", ConfigValue{"tl_car_detection_range", ValueType::Int, 50, 0, 200}, {}, false});
    tl.children.push_back(tl_det);

    Node tl_timing;
    tl_timing.title = "Timing";
    tl_timing.children.push_back(
        Node{"Green Duration", ConfigValue{"green_duration", ValueType::Int, 10, 1, 60}, {}, false});
    tl_timing.children.push_back(
        Node{"Yellow Duration", ConfigValue{"yellow_duration", ValueType::Int, 3, 1, 10}, {}, false});
    tl_timing.children.push_back(
        Node{"All Red Duration",
             ConfigValue{"all_red_duration", ValueType::Float, 0.5f, 0.0f, 5.0f},
             {},
             false});
    tl.children.push_back(tl_timing);

    Node tl_adaptive;
    tl_adaptive.title = "Adaptive";
    tl_adaptive.children.push_back(
        Node{"Enabled", ConfigValue{"tl_adaptive_enabled", ValueType::Bool, true}, {}, false});
    tl_adaptive.children.push_back(
        Node{"Tick Rate", ConfigValue{"tl_adaptive_tick_rate", ValueType::Int, 1, 1, 10}, {}, false});
    tl_adaptive.children.push_back(
        Node{"Min Green Time", ConfigValue{"tl_adaptive_min_green_time", ValueType::Int, 5, 1, 20}, {}, false});
    tl_adaptive.children.push_back(
        Node{"Max Green Time", ConfigValue{"tl_adaptive_max_green_time", ValueType::Int, 20, 5, 60}, {}, false});
    tl.children.push_back(tl_adaptive);

    main_config.children.push_back(tl);

    // Debug Options
    Node debug_options;
    debug_options.title = "Debug Options";

    Node debug_car;
    debug_car.title = "Car";
    debug_car.children.push_back(
        Node{"Detection Arc", ConfigValue{"debug_car_detection_arc", ValueType::Bool, false}, {}, false});
    debug_car.children.push_back(
        Node{"Detection Other Cars", ConfigValue{"debug_car_detection_other_cars", ValueType::Bool, false}, {}, false});
    debug_car.children.push_back(
        Node{"State", ConfigValue{"debug_car_state", ValueType::Bool, false}, {}, false});
    debug_options.children.push_back(debug_car);

    Node debug_tl;
    debug_tl.title = "Traffic Light";
    debug_tl.children.push_back(
        Node{"Car Can Pass", ConfigValue{"debug_tl_car_can_pass", ValueType::Bool, false}, {}, false});
    debug_tl.children.push_back(
        Node{"Car Passed", ConfigValue{"debug_tl_car_passed", ValueType::Bool, false}, {}, false});
    debug_tl.children.push_back(
        Node{"Draw Positions", ConfigValue{"debug_tl_draw_positions", ValueType::Bool, false}, {}, false});
    debug_tl.children.push_back(
        Node{"Traffic Light Wait Positions", ConfigValue{"main_screen_debug_draw_traffic_light_positions", ValueType::Bool, true}, {}, false});
    debug_options.children.push_back(debug_tl);

    root.children.push_back(main_config);
    root.children.push_back(debug_options);

    return root;
}
