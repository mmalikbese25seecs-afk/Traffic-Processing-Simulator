#pragma once
#include "ConfigTypes.hpp"
#include "raylib.h"
#include <vector>
#include <optional>
#include <variant>
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
    Node *node;
    bool is_header;
    ConfigValue *config;
    float indent;
};

void LayoutNodes(
    Node &node,
    std::vector<Field> &outFields,
    float x,
    float &y,
    float indent,
    float width,
    float itemHeight,
    float gap);

void CollectConfigPointers(Node &node, std::vector<ConfigValue *> &out);
ConfigValue *TryReadConfig(Node &node, const std::string &title);
const ConfigValue *TryReadConfig(const Node &node, const std::string &title);
void ApplyTextEdit(ConfigValue &cv, const std::string &text);

float GetConfigFloat(const Node &root, const char *key, float defaultValue);
int GetConfigInt(const Node &root, const char *key, int defaultValue);
bool GetConfigBool(const Node &root, const char *key, bool defaultValue);
