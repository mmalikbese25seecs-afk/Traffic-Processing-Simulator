#pragma once
#include "ConfigTypes.hpp"
#include "raylib.h"
#include <vector>
#include <optional>
#include <string>

struct Node
{
    std::string title;
    std::optional<ConfigValue> value;
    std::vector<Node> children;
    bool collapsed = false;
};

struct Field
{
    Rectangle rect;
    Node* node;
    bool is_header;
    ConfigValue* config;
    float indent;
};

void LayoutNodes(
    Node& node,
    std::vector<Field>& outFields,
    float x,
    float& y,
    float indent,
    float width,
    float itemHeight,
    float gap
);

void CollectConfigPointers(Node& node, std::vector<ConfigValue*>& out);
