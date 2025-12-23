#include "ConfigTree.hpp"
#include <iostream>

void LayoutNodes(
    Node &node,
    std::vector<Field> &outFields,
    float x,
    float &y,
    float indent,
    float width,
    float itemHeight,
    float gap)
{
    Rectangle rect = {x + indent, y, width - indent, itemHeight};

    if (node.value.has_value())
    {
        // leaf node with value
        outFields.push_back({rect, &node, false, &(*node.value), indent});
        y += itemHeight + gap;
    }
    else
    {
        // header node
        outFields.push_back({rect, &node, true, nullptr, indent});
        y += itemHeight + gap;

        if (!node.collapsed)
        {
            for (Node &c : node.children)
                LayoutNodes(c, outFields, x, y, indent + 18.0f, width, itemHeight, gap);
        }
    }
}

// collect all ConfigValue pointers in the tree
void CollectConfigPointers(Node &node, std::vector<ConfigValue *> &out)
{
    if (node.value.has_value())
        out.push_back(&(*node.value));

    for (Node &c : node.children)
        CollectConfigPointers(c, out);
}

ConfigValue *TryReadConfig(Node &node, const std::string &title)
{
    if (node.value.has_value() && node.value->label == title)
        return &(*node.value);

    for (Node &child : node.children)
    {
        if (ConfigValue *found = TryReadConfig(child, title))
            return found;
    }
    return nullptr;
}

// read-only version
const ConfigValue *TryReadConfig(const Node &node, const std::string &title)
{
    if (node.value.has_value() && node.value->label == title)
        return &(*node.value);

    for (const Node &child : node.children)
    {
        if (const ConfigValue *found = TryReadConfig(child, title))
            return found;
    }
    return nullptr;
}

void ApplyTextEdit(ConfigValue &config, const std::string &text)
{
    try
    {
        switch (config.type)
        {
        case ValueType::Int:
        {
            int value = std::stoi(text);
            // clamp
            if (value < static_cast<int>(config.min_value))
                value = static_cast<int>(config.min_value);
            if (value > static_cast<int>(config.max_value))
                value = static_cast<int>(config.max_value);
            config.value = value;
            break;
        }
        case ValueType::Float:
        {
            float value = std::stof(text);
            // clamp
            if (value < config.min_value)
                value = config.min_value;
            if (value > config.max_value)
                value = config.max_value;
            config.value = value;
            break;
        }
        case ValueType::Bool:
        {
            if (text == "true" || text == "1")
                config.value = true;
            else if (text == "false" || text == "0")
                config.value = false;
            break;
        }
        }
    }
    // ignore conversion errors
    catch (...)
    {
    }
}

float GetConfigFloat(const Node &root, const char *key, float defaultValue)
{
    if (auto cfg = TryReadConfig(root, key))
    {
        if (auto f = std::get_if<float>(&cfg->value))
        {
            return *f;
        }
    }
    return defaultValue;
}

int GetConfigInt(const Node &root, const char *key, int defaultValue)
{
    if (auto cfg = TryReadConfig(root, key))
    {
        if (auto i = std::get_if<int>(&cfg->value))
        {
            return *i;
        }
    }
    return defaultValue;
}

bool GetConfigBool(const Node &root, const char *key, bool defaultValue)
{
    if (auto cfg = TryReadConfig(root, key))
    {
        if (auto b = std::get_if<bool>(&cfg->value))
        {
            return *b;
        }
    }
    return defaultValue;
}
