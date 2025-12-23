#pragma once
#include "ConfigTree.hpp"
#include <string>

struct ConfigUIState
{
    ConfigValue* selected = nullptr;
    bool editing = false;
    std::string editBuffer;
    float scrollY = 0.0f;
};

void DrawConfigUI(
    Node& root,
    ConfigUIState& state,
    float contentTop,
    float viewH,
    float contentW
);
