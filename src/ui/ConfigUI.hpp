#pragma once
#include "ConfigTree.hpp"
#include <string>
#include <unordered_map>

struct ConfigUIState
{
    float scrollY = 0.0f;

    Node *hoveredNode = nullptr;
    Node *selectedNode = nullptr;

    // Pending edits: ConfigValue pointer -> edited string
    std::unordered_map<ConfigValue *, std::string> pendingEdits;
};

void DrawConfigUI(Node &root, ConfigUIState &state, float contentTop, float viewH, float contentW);
void ApplyPendingEdits(ConfigUIState &state);
