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
        .value = ConfigValue{"car_spawn_chance", ValueType::Float, 0.5f, 0.0f, 1.0f},
        .children = {},
        .collapsed = false //
    });

    Node car_Collision;
    car_Collision.title = "Collision";

    car_Collision.children.push_back(Node{
        .title = "Radius",
        .value = ConfigValue{"car_collision_radius", ValueType::Int, 50, 0, 200},
        .children = {},
        .collapsed = false //
    });
    car_Collision.children.push_back(Node{
        .title = "Angle (deg)",
        .value = ConfigValue{"car_collision_angle_deg", ValueType::Int, 45, 0, 180},
        .children = {},
        .collapsed = false //
    });
    car_Collision.children.push_back(Node{
        .title = "Detection Range",
        .value = ConfigValue{"car_collision_detection_range", ValueType::Int, 100, 0, 500},
        .children = {},
        .collapsed = false //
    });

    car.children.push_back(car_Collision);
    main_config.children.push_back(car);

    // Traffic Light Config
    Node trafficLightNode;
    trafficLightNode.title = "Traffic Light";

    Node trafficLight_Detection;
    trafficLight_Detection.title = "Detection";
    trafficLight_Detection.children.push_back(Node{
        .title = "Car Detection Range",
        .value = ConfigValue{"traffic_light_car_detection_range", ValueType::Int, 50, 0, 200},
        .children = {},
        .collapsed = false //
    });
    trafficLightNode.children.push_back(trafficLight_Detection);

    Node trafficLight_Timing;
    trafficLight_Timing.title = "Timing";
    trafficLight_Timing.children.push_back(Node{
        .title = "Green Duration",
        .value = ConfigValue{"traffic_light_green_duration", ValueType::Int, 10, 1, 60},
        .children = {},
        .collapsed = false //
    });
    trafficLight_Timing.children.push_back(Node{
        .title = "Yellow Duration",
        .value = ConfigValue{"traffic_light_yellow_duration", ValueType::Int, 3, 1, 10},
        .children = {},
        .collapsed = false //
    });
    trafficLight_Timing.children.push_back(Node{
        .title = "All Red Duration",
        .value = ConfigValue{"traffic_light_all_red_duration", ValueType::Float, 0.5f, 0.0f, 5.0f},
        {},
        false});
    trafficLightNode.children.push_back(trafficLight_Timing);

    Node trafficLight_Adaptive;
    trafficLight_Adaptive.title = "Adaptive";
    trafficLight_Adaptive.children.push_back(Node{
        .title = "Enabled",
        .value = ConfigValue{"traffic_light_adaptive_enabled", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    trafficLight_Adaptive.children.push_back(Node{
        .title = "Refresh Interval",
        .value = ConfigValue{"traffic_light_adaptive_refresh_interval", ValueType::Int, 1, 1, 10},
        .children = {},
        .collapsed = false //
    });
    trafficLight_Adaptive.children.push_back(Node{
        .title = "Min Green Time",
        .value = ConfigValue{"traffic_light_adaptive_min_green_time", ValueType::Int, 5, 1, 20},
        .children = {},
        .collapsed = false //
    });
    trafficLightNode.children.push_back(trafficLight_Adaptive);

    main_config.children.push_back(trafficLightNode);

    // Debug Options
    Node debugNode;
    debugNode.title = "Debug Options";

    Node debug_Car;
    debug_Car.title = "Car";
    debug_Car.children.push_back(Node{
        .title = "Collision Detection Arc",
        .value = ConfigValue{"debug_car_show_collision_arc", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_Car.children.push_back(Node{
        .title = "Detection Other Cars",
        .value = ConfigValue{"debug_car_show_other_car_detection", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_Car.children.push_back(Node{
        .title = "State (Waiting/ Moving)",
        .value = ConfigValue{"debug_car_show_state", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debugNode.children.push_back(debug_Car);

    Node debug_Traffic_Light;
    debug_Traffic_Light.title = "Traffic Light";
    debug_Traffic_Light.children.push_back(Node{
        .title = "Detection Range",
        .value = ConfigValue{"debug_traffic_light_show_detection_range", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debug_Traffic_Light.children.push_back(Node{
        .title = "Stats",
        .value = ConfigValue{"debug_traffic_light_show_stats", ValueType::Bool, true},
        .children = {},
        .collapsed = false //
    });
    debug_Traffic_Light.children.push_back(Node{
        .title = "Traffic Light Wait Positions",
        .value = ConfigValue{"debug_traffic_light_show_wait_positions", ValueType::Bool, false},
        .children = {},
        .collapsed = false //
    });
    debugNode.children.push_back(debug_Traffic_Light);

    root.children.push_back(main_config);
    root.children.push_back(debugNode);

    return root;
}
