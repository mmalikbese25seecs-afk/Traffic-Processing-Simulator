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
    Rectangle r = {x + indent, y, width - indent, itemHeight};

    if (node.value.has_value())
    {
        // leaf node with value
        outFields.push_back({r, &node, false, &(*node.value), indent});
        y += itemHeight + gap;
    }
    else
    {
        // header node
        outFields.push_back({r, &node, true, nullptr, indent});
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
    if (node.value.has_value() && node.title == title)
        return &(*node.value);

    for (const Node &child : node.children)
    {
        const ConfigValue *found = TryReadConfig(child, title);
        if (found)
            return found;
    }

    return nullptr;
}

void ApplyTextEdit(ConfigValue &cv, const std::string &text)
{
    std::cout << "APPLY TEXT EDIT";
    try
    {
        switch (cv.type)
        {
        case ValueType::Int:
        {
            int v = std::stoi(text);
            if (v < static_cast<int>(cv.min_value))
                v = static_cast<int>(cv.min_value);
            if (v > static_cast<int>(cv.max_value))
                v = static_cast<int>(cv.max_value);
            cv.value = v;
            break;
        }
        case ValueType::Float:
        {
            float v = std::stof(text);
            if (v < cv.min_value)
                v = cv.min_value;
            if (v > cv.max_value)
                v = cv.max_value;
            cv.value = v;
            break;
        }
        case ValueType::Bool:
        {
            if (text == "true" || text == "1")
                cv.value = true;
            else if (text == "false" || text == "0")
                cv.value = false;
            break;
        }
        }
    }
    catch (...)
    {
        // ignore invalid input
    }
}
